#include "../internal.h"
#include "import.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdio.h>

#ifdef __APPLE__
#   include <malloc/malloc.h>
#else
#   include <malloc.h>
#endif

#define GLHCK_CHANNEL GLHCK_CHANNEL_IMPORT

/* \brief get file extension */
static const char* getExt(const char *file)
{
   const char *ext = strrchr(file, '.');
   if (!ext) return NULL;
   return ext + 1;
}

static int buildModel(glhckObject *object, size_t numIndices, size_t numVertices,
      const glhckImportIndexData *indices, const glhckImportVertexData *vertexData,
      glhckGeometryIndexType itype, glhckGeometryVertexType vtype, unsigned int flags)
{
   unsigned int geometryType = GLHCK_TRIANGLE_STRIP;
   size_t numStrippedIndices = 0;
   glhckImportIndexData *stripIndices = NULL;
   (void)flags;

   if (!numVertices)
      return RETURN_OK;

   /* triangle strip geometry */
   if (!(stripIndices = _glhckTriStrip(indices, numIndices, &numStrippedIndices))) {
      /* failed, use non stripped geometry */
      geometryType       = GLHCK_TRIANGLES;
      numStrippedIndices = numIndices;
   }

   /* set geometry */
   glhckObjectInsertIndices(object, itype, (stripIndices?stripIndices:indices), numStrippedIndices);
   glhckObjectInsertVertices(object, vtype, vertexData, numVertices);
   object->geometry->type = geometryType;
   IFDO(_glhckFree, stripIndices);
   return RETURN_OK;
}

static void joinMesh(const struct aiMesh *mesh, size_t indexOffset,
      glhckImportIndexData *indices, glhckImportVertexData *vertexData,
      glhckAtlas *atlas, glhckTexture *texture)
{
   unsigned int f, i, skip;
   glhckImportIndexData index;
   const struct aiFace *face;

   for (f = 0, skip = 0; f != mesh->mNumFaces; ++f) {
      face = &mesh->mFaces[f];
      if (!face) continue;

      for (i = 0; i != face->mNumIndices; ++i) {
         index = face->mIndices[i];

         if (mesh->mVertices) {
            vertexData[index].vertex.x = mesh->mVertices[index].x;
            vertexData[index].vertex.y = mesh->mVertices[index].y;
            vertexData[index].vertex.z = mesh->mVertices[index].z*-1;
         }

         if (mesh->mNormals) {
            vertexData[index].normal.x = mesh->mNormals[index].x;
            vertexData[index].normal.y = mesh->mNormals[index].y;
            vertexData[index].normal.z = mesh->mNormals[index].z*-1;
         }

         if (mesh->mTextureCoords[0]) {
            vertexData[index].coord.x = mesh->mTextureCoords[0][index].x;
            vertexData[index].coord.y = mesh->mTextureCoords[0][index].y;

            /* fix coords */
            if (vertexData[index].coord.x < 0.0f)
               vertexData[index].coord.x += 1;
            if (vertexData[index].coord.y < 0.0f)
               vertexData[index].coord.y += 1;
         }

         /* offset texture coords to fit atlas texture */
         if (atlas && texture) {
            kmVec2 coord;
            coord.x = vertexData[index].coord.x;
            coord.y = vertexData[index].coord.y;
            glhckAtlasTransformCoordinates(atlas, texture, &coord, &coord);
            vertexData[index].coord.x = coord.x;
            vertexData[index].coord.y = coord.y;
         }

         indices[skip+i] = indexOffset + index;
      } skip += face->mNumIndices;
   }
}

glhckTexture* textureFromMaterial(const char *file, const struct aiMaterial *mtl)
{
   glhckTexture *texture;
   struct aiString textureName;
   enum aiTextureMapping textureMapping;
   enum aiTextureOp op;
   enum aiTextureMapMode textureMapMode[3] = {0,0,0};
   unsigned int uvwIndex, flags;
   float blend;
   char *texturePath;
   glhckTextureParameters params;

   if (!aiGetMaterialTextureCount(mtl, aiTextureType_DIFFUSE))
      return NULL;

   if (aiGetMaterialTexture(mtl, aiTextureType_DIFFUSE, 0,
            &textureName, &textureMapping, &uvwIndex, &blend, &op,
            textureMapMode, &flags) != AI_SUCCESS)
      return NULL;

   memcpy(&params, glhckTextureDefaultParameters(), sizeof(glhckTextureParameters));
   switch (textureMapMode[0]) {
      case aiTextureMapMode_Clamp:
      case aiTextureMapMode_Decal:
         params.wrapR = GLHCK_WRAP_CLAMP_TO_EDGE;
         params.wrapS = GLHCK_WRAP_CLAMP_TO_EDGE;
         params.wrapT = GLHCK_WRAP_CLAMP_TO_EDGE;
         break;
      case aiTextureMapMode_Mirror:
         params.wrapR = GLHCK_WRAP_MIRRORED_REPEAT;
         params.wrapS = GLHCK_WRAP_MIRRORED_REPEAT;
         params.wrapT = GLHCK_WRAP_MIRRORED_REPEAT;
      break;
      default:break;
   }

   if (!(texturePath = _glhckImportTexturePath(textureName.data, file)))
      return NULL;

   DEBUG(0, "%s", texturePath);
   texture = glhckTextureNew(texturePath, 0, &params);
   free(texturePath);
   return texture;
}

static int processModel(const char *file, glhckObject *object,
      glhckObject *current, const struct aiScene *sc, const struct aiNode *nd,
      glhckGeometryIndexType itype, glhckGeometryVertexType vtype, unsigned int flags)
{
   unsigned int m, f;
   size_t numVertices = 0, numIndices = 0;
   size_t ioffset, voffset;
   glhckImportIndexData *indices = NULL;
   glhckImportVertexData *vertexData = NULL;
   glhckTexture **textureList = NULL, *texture;
   glhckAtlas *atlas = NULL;
   const struct aiMesh *mesh;
   const struct aiFace *face;
   int canFreeCurrent = 0;
   int hasTexture = 0;

   /* combine && atlas loading path */
   if (flags & GLHCK_MODEL_JOIN) {
      /* prepare atlas for texture combining */
      if (!(atlas = glhckAtlasNew()))
         goto assimp_no_memory;

      /* texturelist for offseting coordinates */
      if (!(textureList = _glhckCalloc(nd->mNumMeshes, sizeof(_glhckTexture*))))
         goto assimp_no_memory;

      /* gather statistics */
      for (m = 0; m != nd->mNumMeshes; ++m) {
         mesh = sc->mMeshes[nd->mMeshes[m]];
         if (!mesh->mVertices) continue;

         for (f = 0; f != mesh->mNumFaces; ++f) {
            face = &mesh->mFaces[f];
            if (!face) goto fail;
            numIndices += face->mNumIndices;
         }
         numVertices += mesh->mNumVertices;

         if ((texture = textureFromMaterial(file, sc->mMaterials[mesh->mMaterialIndex]))) {
            glhckAtlasInsertTexture(atlas, texture);
            glhckTextureFree(texture);
            textureList[m] = texture;
            hasTexture = 1;
         }
      }

      /* allocate vertices */
      if (!(vertexData = _glhckCalloc(numVertices, sizeof(glhckImportVertexData))))
         goto assimp_no_memory;

      /* allocate indices */
      if (!(indices = _glhckMalloc(numIndices * sizeof(glhckImportIndexData))))
         goto assimp_no_memory;

      /* pack combined textures */
      if (hasTexture) {
         if (glhckAtlasPack(atlas, 1, 0) != RETURN_OK)
            goto fail;
      } else {
         NULLDO(glhckAtlasFree, atlas);
         NULLDO(_glhckFree, textureList);
      }

      /* join vertex data */
      for (m = 0, ioffset = 0, voffset = 0; m != nd->mNumMeshes; ++m) {
         mesh = sc->mMeshes[nd->mMeshes[m]];
         if (!mesh->mVertices) continue;
         if (textureList) texture = textureList[m];
         else texture = NULL;

         joinMesh(mesh, voffset, indices+ioffset, vertexData+voffset, atlas, texture);

         for (f = 0; f != mesh->mNumFaces; ++f) {
            face = &mesh->mFaces[f];
            if (!face) goto fail;
            ioffset += face->mNumIndices;
         }
         voffset += mesh->mNumVertices;
      }

      /* finally build the model */
      if (buildModel(current, numIndices,  numVertices,
               indices, vertexData, itype, vtype, flags) == RETURN_OK) {
         if (hasTexture) glhckObjectTexture(current, glhckAtlasGetTexture(atlas));
         if (!(current = glhckObjectNew())) goto fail;
         glhckObjectAddChildren(object, current);
         glhckObjectFree(current);
         canFreeCurrent = 1;
      }

      /* free stuff */
      IFDO(glhckAtlasFree, atlas);
      IFDO(_glhckFree, textureList);
      NULLDO(_glhckFree, vertexData);
      NULLDO(_glhckFree, indices);
   } else {
      /* default loading path */
      for (m = 0, ioffset = 0, voffset = 0; m != nd->mNumMeshes; ++m) {
         mesh = sc->mMeshes[nd->mMeshes[m]];
         if (!mesh->mVertices) continue;

         /* gather statistics */
         numIndices = 0;
         for (f = 0; f != mesh->mNumFaces; ++f) {
            face = &mesh->mFaces[f];
            if (!face) goto fail;
            numIndices += face->mNumIndices;
         }
         numVertices = mesh->mNumVertices;

         /* get texture */
         hasTexture = 0;
         if ((texture = textureFromMaterial(file, sc->mMaterials[mesh->mMaterialIndex]))) {
            hasTexture = 1;
         }

         /* allocate vertices */
         if (!(vertexData = _glhckCalloc(numVertices, sizeof(glhckImportVertexData))))
            goto assimp_no_memory;

         /* allocate indices */
         if (!(indices = _glhckMalloc(numIndices * sizeof(glhckImportIndexData))))
            goto assimp_no_memory;

         /* fill arrays */
         joinMesh(mesh, 0, indices, vertexData, NULL, NULL);

         /* build model */
         if (buildModel(current, numIndices,  numVertices,
                  indices, vertexData, itype, vtype, flags) == RETURN_OK) {
            if (hasTexture) glhckObjectTexture(current, texture);
            if (!(current = glhckObjectNew())) goto fail;
            glhckObjectAddChildren(object, current);
            glhckObjectFree(current);
            canFreeCurrent = 1;
         }

         /* free stuff */
         NULLDO(_glhckFree, vertexData);
         NULLDO(_glhckFree, indices);
         IFDO(glhckTextureFree, texture);
      }
   }

   /* process childrens */
   for (m = 0; m != nd->mNumChildren; ++m) {
      if (processModel(file, object, current, sc, nd->mChildren[m],
               itype, vtype, flags) == RETURN_OK) {
         if (!(current = glhckObjectNew())) goto fail;
         glhckObjectAddChildren(object, current);
         glhckObjectFree(current);
         canFreeCurrent = 1;
      }
   }

   /* we din't do anything to the next
    * allocated object, so free it */
   if (canFreeCurrent) glhckObjectRemoveFromParent(current);
   return RETURN_OK;

assimp_no_memory:
   DEBUG(GLHCK_DBG_ERROR, "Assimp not enough memory.");
fail:
   IFDO(_glhckFree, vertexData);
   IFDO(_glhckFree, indices);
   IFDO(_glhckFree, textureList);
   IFDO(glhckAtlasFree, atlas);
   if (canFreeCurrent) glhckObjectRemoveFromParent(current);
   return RETURN_FAIL;
}

/* \brief check if file is a Assimp supported file */
int _glhckFormatAssimp(const char *file)
{
   int ret;
   CALL(0, "%s", file);
   ret = (aiIsExtensionSupported(getExt(file))?RETURN_OK:RETURN_FAIL);
   RET(0, "%d", ret);
   return ret;
}

/* \brief import Assimp file */
int _glhckImportAssimp(glhckObject *object, const char *file, unsigned int flags,
      glhckGeometryIndexType itype, glhckGeometryVertexType vtype)
{
   const struct aiScene *scene;
   glhckObject *first = NULL;
   unsigned int aflags;
   CALL(0, "%p, %s, %d", object, file, flags);

   /* import the model using assimp
    * TODO: make import hints tunable?
    * Needs changes to import protocol! */
   aflags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph;
   scene = aiImportFile(file, aflags);
   if (!scene) goto assimp_fail;

   /* mark ourself as special root object.
    * this makes most functions called on root object echo to children */
   object->flags |= GLHCK_OBJECT_ROOT;

   /* this is going to be the first object in mesh,
    * the object returned by this importer is just invisible root object. */
   if (!(first = glhckObjectNew())) goto fail;
   glhckObjectAddChildren(object, first);
   glhckObjectFree(first);

   /* process the model */
   if (processModel(file, object, first, scene, scene->mRootNode,
            itype, vtype, flags) != RETURN_OK)
      goto fail;

   /* close file */
   NULLDO(aiReleaseImport, scene);
   RET(0, "%d", RETURN_OK);
   return RETURN_OK;

assimp_fail:
   DEBUG(GLHCK_DBG_ERROR, aiGetErrorString());
fail:
   IFDO(aiReleaseImport, scene);
   IFDO(glhckObjectFree, first);
   RET(0, "%d", RETURN_FAIL);
   return RETURN_FAIL;
}

/* vim: set ts=8 sw=3 tw=0 :*/
