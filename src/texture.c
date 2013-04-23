#include "internal.h"
#include "import/import.h"
#include "imghck.h"
#include <stdio.h>            /* for file io */
#include <assert.h>           /* for assert */

/* tracing channel for this file */
#define GLHCK_CHANNEL GLHCK_CHANNEL_TEXTURE

/* ---- TEXTURE CACHE ---- */

/* \brief check if texture is in cache, returns reference if found */
static glhckTexture* _glhckTextureCacheCheck(const char *file)
{
   glhckTexture *cache;
   CALL(1, "%s", file);

   if (!file)
      goto nothing;

   for (cache = GLHCKW()->texture; cache; cache = cache->next) {
      if (cache->file && !strcmp(cache->file, file))
         return glhckTextureRef(cache);
   }

nothing:
   RET(1, "%p", NULL);
   return NULL;
}

/* \brief assign texture to draw list */
inline void _glhckTextureInsertToQueue(glhckTexture *object)
{
   __GLHCKtextureQueue *textures;
   unsigned int i;

   textures = &GLHCKRD()->textures;

   /* check duplicate */
   for (i = 0; i != textures->count; ++i)
      if (textures->queue[i] == object) return;

   /* need alloc dynamically more? */
   if (textures->allocated <= textures->count+1) {
      textures->queue = _glhckRealloc(textures->queue,
            textures->allocated,
            textures->allocated + GLHCK_QUEUE_ALLOC_STEP,
            sizeof(glhckTexture*));

      /* epic fail here */
      if (!textures->queue) return;
      textures->allocated += GLHCK_QUEUE_ALLOC_STEP;
   }

   /* assign the texture to list */
   textures->queue[textures->count] = glhckTextureRef(object);
   textures->count++;
}

/* \brief set texture data.
 * NOTE: internal function, so data isn't copied. */
static void _glhckTextureSetData(glhckTexture *object, void *data)
{
   CALL(1, "%p, %p", object, data);
   assert(object);
   IFDO(_glhckFree, object->data);
   object->data = data;
}

/* \brief guess size for texture */
inline int _glhckSizeForTexture(glhckTextureTarget target, int width, int height, int depth, glhckTextureFormat format, glhckDataType type)
{
   int size = 0;
   size_t b = 0;

   switch (type) {
      case GLHCK_DATA_BYTE:
      case GLHCK_DATA_UNSIGNED_BYTE:
         b = sizeof(unsigned char);
         break;
      case GLHCK_DATA_SHORT:
      case GLHCK_DATA_UNSIGNED_SHORT:
         b = sizeof(unsigned short);
         break;
      case GLHCK_DATA_INT:
      case GLHCK_DATA_UNSIGNED_INT:
         b = sizeof(unsigned int);
         break;
      case GLHCK_DATA_FLOAT:
         b = sizeof(float);
         break;
      case GLHCK_DATA_COMPRESSED:
         assert(0 && "This should not never happen");break;
         break;
      default:assert(0 && "Datatype size not known");break;
   }

   switch (target) {
      case GLHCK_TEXTURE_1D:
         size = width * _glhckNumChannels(format) * b;
         break;
      case GLHCK_TEXTURE_2D:
      case GLHCK_TEXTURE_CUBE_MAP:
         size = width * height * _glhckNumChannels(format) * b;
         break;
      case GLHCK_TEXTURE_3D:
         size = width * height * depth * _glhckNumChannels(format) * b;
         break;
      default:assert(0 && "Target's size calculation not implemented");break;
   }
   return size;
}

/* \brief returns number of channels needed for texture format */
inline unsigned int _glhckNumChannels(glhckTextureFormat format)
{
   switch (format) {
      case GLHCK_RED:
      case GLHCK_ALPHA:
      case GLHCK_LUMINANCE:
      case GLHCK_DEPTH_COMPONENT:
      case GLHCK_DEPTH_COMPONENT16:
      case GLHCK_DEPTH_COMPONENT24:
      case GLHCK_DEPTH_COMPONENT32:
      case GLHCK_DEPTH_STENCIL:
         return 1;
      case GLHCK_RG:
      case GLHCK_LUMINANCE_ALPHA:
         return 2;
      case GLHCK_RGB:
      case GLHCK_BGR:
      case GLHCK_COMPRESSED_RGB_DXT1:
         return 3;
      case GLHCK_RGBA:
      case GLHCK_BGRA:
      case GLHCK_COMPRESSED_RGBA_DXT5:
         return 4;
      default:break;
   }
   assert(0 && "Num channels not implemented for texture format");
   return 0;
}

/* \brief returns if format is compressed format */
inline int _glhckIsCompressedFormat(glhckTextureFormat format)
{
   switch (format) {
      case GLHCK_COMPRESSED_RGB_DXT1:
      case GLHCK_COMPRESSED_RGBA_DXT5:
         return 1;
      default:break;
   }
   return 0;
}

/* \brief compress image data internally */
static void* _glhckTextureCompress(glhckTextureCompression compression, int width, int height, int depth, glhckTextureFormat format, glhckDataType type, const void *data, int *size, glhckTextureFormat *outFormat)
{
   void *compressed = NULL;
   CALL(0, "%d, %d, %d, %d, %d, %d, %p, %p, %p", compression, width, height, depth, format, type, data, size, outFormat);
   if (size)      *size = 0;
   if (outFormat) *outFormat = 0;

   /* NULL data, just return */
   if (!data) goto fail;

   /* check if already compressed */
   if (_glhckIsCompressedFormat(format))
      goto already_compressed;

   /* DXT compression requested */
   if (compression == GLHCK_COMPRESSION_DXT) {
      if ((_glhckNumChannels(format) & 1) == 1) {
         /* RGB */
         if (!(compressed = _glhckMalloc(imghckSizeForDXT1(width, height))))
            goto out_of_memory;

         imghckConvertToDXT1(compressed, data, width, height, _glhckNumChannels(format));
         if (size)      *size      = imghckSizeForDXT1(width, height);
         if (outFormat) *outFormat = GLHCK_COMPRESSED_RGB_DXT1;
         DEBUG(GLHCK_DBG_CRAP, "Image data converted to DXT1");
      } else {
         /* RGBA */
         if (!(compressed = _glhckMalloc(imghckSizeForDXT5(width, height))))
            goto out_of_memory;

         imghckConvertToDXT5(compressed, data, width, height, _glhckNumChannels(format));
         if (size)      *size      = imghckSizeForDXT5(width, height);
         if (outFormat) *outFormat = GLHCK_COMPRESSED_RGBA_DXT5;
         DEBUG(GLHCK_DBG_CRAP, "Image data converted to DXT5");
      }
   } else {
      DEBUG(GLHCK_DBG_WARNING, "Compression type not implemented or supported for the texture format/datatype combination.");
   }

   RET(0, "%p", compressed);
   return compressed;

already_compressed:
   DEBUG(GLHCK_DBG_WARNING, "Image data is already compressed");
   goto fail;
out_of_memory:
   DEBUG(GLHCK_DBG_ERROR, "Out of memory");
fail:
   IFDO(_glhckFree, compressed);
   RET(0, "%p", NULL);
   return NULL;
}

/* ---- PUBLIC API ---- */

/* \brief Allocate texture
 * Takes filename as argument, pass NULL to use user data */
GLHCKAPI glhckTexture* glhckTextureNew(const char *file, unsigned int importFlags, const glhckTextureParameters *params)
{
   glhckTexture *object;
   CALL(0, "%s, %u, %p", file, importFlags, params);

   /* check if texture is in cache */
   if ((object = _glhckTextureCacheCheck(file)))
      goto success;

   /* allocate texture */
   if (!(object = (glhckTexture*)_glhckCalloc(1, sizeof(glhckTexture))))
      goto fail;

   /* increase reference */
   object->refCounter++;

   /* default target type */
   object->target = GLHCK_TEXTURE_2D;

   /* If file is passed, then try import it */
   if (file) {
      /* copy filename */
      if (!(object->file = _glhckStrdup(file)))
         goto fail;

      /* import image */
      if (_glhckImportImage(object, file, importFlags) != RETURN_OK)
         goto fail;

      /* apply texture parameters */
      glhckTextureParameter(object, params);
   }

   /* insert to world */
   _glhckWorldInsert(texture, object, glhckTexture*);

success:
   RET(0, "%p", object);
   return object;

fail:
   IFDO(glhckTextureFree, object);
   RET(0, "%p", NULL);
   return NULL;
}

/* \brief copy texture */
GLHCKAPI glhckTexture* glhckTextureCopy(glhckTexture *src)
{
   glhckTexture *object;
   CALL(0, "%p", src);
   assert(src);

   /* allocate texture */
   if (!(object = (glhckTexture*)_glhckCalloc(1, sizeof(glhckTexture))))
      goto fail;

   /* increase reference */
   object->refCounter++;

   /* copy */
   GLHCKRA()->textureGenerate(1, &object->object);

   if (src->file)
      object->file = _glhckStrdup(src->file);

   glhckTextureCreate(object, src->target, 0, src->width, src->height, src->depth, 0,
         src->format, src->type, src->size, src->data);

   /* insert to world */
   _glhckWorldInsert(texture, object, glhckTexture*);

   /* return texture */
   RET(0, "%p", object);
   return object;

fail:
   IFDO(glhckTextureFree, object);
   RET(0, "%p", NULL);
   return NULL;
}

/* \brief reference texture */
GLHCKAPI glhckTexture* glhckTextureRef(glhckTexture *object)
{
   CALL(3, "%p", object);
   assert(object);

   /* increase ref counter */
   object->refCounter++;

   /* return reference */
   RET(3, "%p", object);
   return object;
}

/* \brief free texture */
GLHCKAPI unsigned int glhckTextureFree(glhckTexture *object)
{
   unsigned int i;
   if (!glhckInitialized()) return 0;
   CALL(FREE_CALL_PRIO(object), "%p", object);
   assert(object);

   /* there is still references to this object alive */
   if (--object->refCounter != 0) goto success;

   DEBUG(GLHCK_DBG_CRAP, "FREE(%p) %dx%d %.2f MiB", object,
         object->width, object->height, (float)object->size / 1048576);

   /* unbind from active slot */
   for (i = 0; i != GLHCK_MAX_ACTIVE_TEXTURE; ++i) {
      if (GLHCKRD()->texture[i][object->target] == object)
         glhckTextureUnbind(object->target);
   }

   /* delete texture if there is one */
   if (object->object)
      GLHCKRA()->textureDelete(1, &object->object);

   /* free */
   IFDO(_glhckFree, object->file);
   _glhckTextureSetData(object, NULL);

   /* remove from world */
   _glhckWorldRemove(texture, object, glhckTexture*);

   /* free */
   NULLDO(_glhckFree, object);

success:
   RET(FREE_RET_PRIO(object), "%d", object?object->refCounter:0);
   return object?object->refCounter:0;
}

/* \brief apply texture parameters. */
GLHCKAPI void glhckTextureParameter(glhckTexture *object, const glhckTextureParameters *params)
{
   int size;
   void *data = NULL;
   glhckTexture *old = NULL;
   glhckTextureFormat outFormat;

   /* copy texture parameters over */
   memcpy(&object->params, (params?params:glhckTextureDefaultParameters()), sizeof(glhckTextureParameters));
   params = &object->params;

   /* will do compression if requested */
   if (params->compression != GLHCK_COMPRESSION_NONE) {
      if ((data = _glhckTextureCompress(params->compression, object->width, object->height, object->depth, object->format,
                  object->type, object->data, &size, &outFormat))) {
         glhckTextureRecreate(object, outFormat, GLHCK_DATA_UNSIGNED_BYTE, size, data);
         _glhckFree(data);
      }
   }

   /* push texture parameters to renderer */
   old = glhckTextureCurrentForTarget(object->target);
   glhckTextureBind(object);
   GLHCKRA()->textureParameter(object->target, params);
   if (params->mipmap) GLHCKRA()->textureGenerateMipmap(object->target);
   if (old) glhckTextureBind(old);
}

/* \brief return default texture parameters */
GLHCKAPI const glhckTextureParameters* glhckTextureDefaultParameters(void)
{
   static glhckTextureParameters defaultParameters = {
      .minLod      = -1000.0f,
      .maxLod      = 1000.0f,
      .biasLod     = 0.0f,
      .baseLevel   = 0,
      .maxLevel    = 1000,
      .wrapS       = GLHCK_WRAP_REPEAT,
      .wrapT       = GLHCK_WRAP_REPEAT,
      .wrapR       = GLHCK_WRAP_REPEAT,
      .minFilter   = GLHCK_FILTER_NEAREST_MIPMAP_LINEAR,
      .magFilter   = GLHCK_FILTER_LINEAR,
      .compareMode = GLHCK_COMPARE_NONE,
      .compareFunc = GLHCK_COMPARE_LEQUAL,
      .compression = GLHCK_COMPRESSION_NONE,
      .mipmap      = 1,
   };
   return &defaultParameters;
}

/* \brief get texture's data */
GLHCKAPI const void* glhckTextureGetData(glhckTexture *object, int *size)
{
   CALL(0, "%p, %p", object, size);
   assert(object);
   if (size) *size = object->size;
   RET(0, "%p", object->data);
   return object->data;
}

/* \brief get texture's information */
GLHCKAPI void glhckTextureGetInformation(glhckTexture *object, glhckTextureTarget *target, int *width, int *height, int *depth, int *border, glhckTextureFormat *format, glhckDataType *type)
{
   CALL(0, "%p, %p, %p, %p, %p, %p, %p, %p", object, target, width, height, depth, border, format, type);
   assert(object);
   if (target) *target = object->target;
   if (width) *width = object->width;
   if (height) *height = object->height;
   if (depth) *depth = object->depth;
   if (border) *border = 0;
   if (format) *format = object->format;
   if (type) *type = object->type;
}

/* \brief create texture manually. */
GLHCKAPI int glhckTextureCreate(glhckTexture *object, glhckTextureTarget target, int level, int width, int height, int depth, int border, glhckTextureFormat format, glhckDataType type, int size, const void *data)
{
   void *copied = NULL;
   glhckTexture *old = NULL;
   CALL(0, "%p, %d, %d, %d, %d, %d, %d, %d, %d, %d, %p", object, target, level, width, height, depth, border, format, type, size, data);
   assert(object);
   assert(level == 0 && "For now the level parameter must be 0!");
   assert(border == 0 && "For now the border parameter must be 0!");

   /* check the true data size, if not provided */
   if (!size) {
      size = _glhckSizeForTexture(target, width, height, depth, format, type);
   }

   /* create texture */
   if (!object->object) GLHCKRA()->textureGenerate(1, &object->object);
   if (!object->object) goto fail;

   /* set texture type */
   object->target = target;

   old = glhckTextureCurrentForTarget(object->target);
   glhckTextureBind(object);
   GLHCKRA()->textureImage(target, level, width, height, depth, border, format, type, size, data);
   if (old) glhckTextureBind(old);

   /* copy data */
   if (data) {
      if (!(copied = _glhckCopy(data, size)))
         goto fail;
   }

   /* set the copied data */
   _glhckTextureSetData(object, copied);

   /* set rest */
   object->width  = width;
   object->height = height;
   object->depth  = depth;
   object->format = format;
   object->type   = type;
   object->size   = size;

   DEBUG(GLHCK_DBG_CRAP, "NEW(%p) %dx%dx%d %.2f MiB", object,
         object->width, object->height, object->depth, (float)object->size / 1048576);

   RET(0, "%d", RETURN_OK);
   return RETURN_OK;

fail:
   RET(0, "%d", RETURN_FAIL);
   return RETURN_FAIL;
}

/* \brief recreate texture with new data.
 * dimensions of texture must be the same. */
GLHCKAPI int glhckTextureRecreate(glhckTexture *object, glhckTextureFormat format, glhckDataType type, int size, const void *data)
{
   /* check the true data size, if not provided */
   if (!size) {
      size = _glhckSizeForTexture(object->target, object->width, object->height, object->depth, format, type);
   }

   return glhckTextureCreate(object, object->target, 0, object->width, object->height,
         object->depth, 0, format, type, size, data);
}

/* \brief fill subdata to texture */
GLHCKAPI void glhckTextureFill(glhckTexture *object, int level, int x, int y, int z, int width, int height, int depth, glhckTextureFormat format, glhckDataType type, int size, const void *data)
{
   glhckTexture *old;
   CALL(2, "%p, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %p", object, level, x, y, z, width, height, depth, format, type, size, data);
   assert(object);
   old = glhckTextureCurrentForTarget(object->target);
   glhckTextureBind(object);
   GLHCKRA()->textureFill(object->target, level, x, y, z, width, height, depth, format, type, size, data);
   if (old) glhckTextureBind(old);
}

/* \brief save texture to file in TGA format */
GLHCKAPI int glhckTextureSave(glhckTexture *object, const char *path)
{
   FILE *f = NULL;
   CALL(0, "%p, %s", object, path);
   assert(object);

   /* not correct texture format for saving */
   if (object->format != GLHCK_RGBA)
      goto format_fail;

   DEBUG(GLHCK_DBG_CRAP,
         "\2Save \3%d\5x\3%d \5[\4%s, \3%d\5]",
         object->width, object->height,
         object->format==GLHCK_RGBA?"RGBA":
         object->format==GLHCK_RGB?"RGB":
         object->format==GLHCK_LUMINANCE_ALPHA?
         "LUMINANCE ALPHA":"LUMINANCE",
         _glhckNumChannels(object->format));

#if 0
   /* open for read */
   if (!(f = fopen(path, "wb")))
      goto fail;

   /* dump raw data */
   fwrite(object->data, 1, object->size, f);
   NULLDO(fclose, f);
#endif

   RET(0, "%d", RETURN_OK);
   return RETURN_OK;

format_fail:
   _glhckPuts("\1Err.. Sorry only RGBA texture save for now.");
//fail:
   IFDO(fclose, f);
   RET(0, "%d", RETURN_FAIL);
   return RETURN_FAIL;
}

/* \brief get current active texture for type */
GLHCKAPI glhckTexture* glhckTextureCurrentForTarget(glhckTextureTarget target)
{
   CALL(2, "%d", target);
   RET(2, "%p", GLHCKRD()->texture[GLHCKRD()->activeTexture][target]);
   return GLHCKRD()->texture[GLHCKRD()->activeTexture][target];
}

/* \brief active texture index */
GLHCKAPI void glhckTextureActive(unsigned int index)
{
   assert(index < GLHCK_MAX_ACTIVE_TEXTURE && "Tried to active bigger texture index than GLhck supports");
   if (GLHCKRD()->activeTexture == index)
      return;

   GLHCKRA()->textureActive(index);
   GLHCKRD()->activeTexture = index;
}

/* \brief bind texture */
GLHCKAPI void glhckTextureBind(glhckTexture *object)
{
   CALL(2, "%p", object);
   assert(object);
   if (GLHCKRD()->texture[GLHCKRD()->activeTexture][object->target] == object) return;
   GLHCKRA()->textureBind(object->target, object->object);
   GLHCKRD()->texture[GLHCKRD()->activeTexture][object->target] = object;
}

/* \brief unbind texture */
GLHCKAPI void glhckTextureUnbind(glhckTextureTarget target)
{
   CALL(2, "%d", target);
   if (!GLHCKRD()->texture[GLHCKRD()->activeTexture][target]) return;
   GLHCKRA()->textureBind(target, 0);
   GLHCKRD()->texture[GLHCKRD()->activeTexture][target] = NULL;
}

/* vim: set ts=8 sw=3 tw=0 :*/
