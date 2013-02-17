#include "../internal.h"
#include <limits.h> /* for limits */
#include <stdio.h>  /* for snprintf */

#define GLHCK_CHANNEL GLHCK_CHANNEL_RENDER
#include "OpenGLHelper.h"

/*
 * glhck to OpenGL mappings
 */

/* \brief map glhck render property to OpenGL render property */
GLenum glhRenderPropertyForGlhckProperty(glhckRenderProperty property)
{
   switch (property) {
      case GLHCK_MAX_TEXTURE_SIZE:
         return GL_MAX_TEXTURE_SIZE;
      case GLHCK_MAX_CUBE_MAP_TEXTURE_SIZE:
         return GL_MAX_CUBE_MAP_TEXTURE_SIZE;
      case GLHCK_MAX_VERTEX_ATTRIBS:
         return GL_MAX_VERTEX_ATTRIBS;
      case GLHCK_MAX_VERTEX_UNIFORM_VECTORS:
         return GL_MAX_VERTEX_UNIFORM_VECTORS;
      case GLHCK_MAX_VARYING_VECTORS:
         return GL_MAX_VARYING_VECTORS;
      case GLHCK_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
         return GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
      case GLHCK_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
         return GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS;
      case GLHCK_MAX_TEXTURE_IMAGE_UNITS:
         return GL_MAX_TEXTURE_IMAGE_UNITS;
      case GLHCK_MAX_FRAGMENT_UNIFORM_VECTORS:
         return GL_MAX_FRAGMENT_UNIFORM_VECTORS;
      case GLHCK_MAX_RENDERBUFFER_SIZE:
         return GL_MAX_RENDERBUFFER_SIZE;
      case GLHCK_MAX_VIEWPORT_DIMS:
         return GL_MAX_VIEWPORT_DIMS;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck face type to OpenGL face type */
GLenum glhCullFaceTypeForGlhckType(glhckCullFaceType type)
{
   switch (type) {
      case GLHCK_CULL_FRONT:
         return GL_FRONT;
      case GLHCK_CULL_BACK:
         return GL_BACK;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck geometry constant to OpenGL geometry constant */
GLenum glhGeometryTypeForGlhckType(glhckGeometryType type)
{
   switch (type) {
      case GLHCK_POINTS:
         return GL_POINTS;
      case GLHCK_LINES:
         return GL_LINES;
      case GLHCK_LINE_LOOP:
         return GL_LINE_LOOP;
      case GLHCK_LINE_STRIP:
         return GL_LINE_STRIP;
      case GLHCK_TRIANGLES:
         return GL_TRIANGLES;
      case GLHCK_TRIANGLE_STRIP:
         return GL_TRIANGLE_STRIP;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck texture format to OpenGL texture format */
GLenum glhTextureTypeForGlhckType(glhckTextureType type)
{
   switch (type) {
      case GLHCK_TEXTURE_1D:
         return GL_TEXTURE_1D;
      case GLHCK_TEXTURE_2D:
         return GL_TEXTURE_2D;
      case GLHCK_TEXTURE_3D:
         return GL_TEXTURE_3D;
      case GLHCK_TEXTURE_CUBE_MAP:
         return GL_TEXTURE_CUBE_MAP;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck texture format to OpenGL texture format */
GLenum glhTextureFormatForGlhckFormat(glhckTextureFormat format)
{
   switch (format) {
      case GLHCK_RED:
         return GL_RED;
      case GLHCK_GREEN:
         return GL_GREEN;
      case GLHCK_BLUE:
         return GL_BLUE;
      case GLHCK_ALPHA:
         return GL_ALPHA;
      case GLHCK_LUMINANCE:
         return GL_LUMINANCE;
      case GLHCK_LUMINANCE_ALPHA:
         return GL_LUMINANCE_ALPHA;
      case GLHCK_RGB:
         return GL_RGB;
      case GLHCK_RGBA:
         return GL_RGBA;
      case GLHCK_DEPTH_COMPONENT:
         return GL_DEPTH_COMPONENT;
      case GLHCK_DEPTH_COMPONENT16:
         return GL_DEPTH_COMPONENT16;
      case GLHCK_DEPTH_COMPONENT24:
         return GL_DEPTH_COMPONENT24;
      case GLHCK_DEPTH_COMPONENT32:
         return GL_DEPTH_COMPONENT32;
      case GLHCK_COMPRESSED_RGBA_DXT5:
         return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      case GLHCK_COMPRESSED_RGB_DXT1:
         return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck blending mode to OpenGL blending mode */
GLenum glhBlendingModeForGlhckMode(glhckBlendingMode mode)
{
   switch (mode) {
      case GLHCK_ZERO:
         return GL_ZERO;
      case GLHCK_ONE:
         return GL_ONE;
      case GLHCK_SRC_COLOR:
         return GL_SRC_COLOR;
      case GLHCK_ONE_MINUS_SRC_COLOR:
         return GL_ONE_MINUS_SRC_COLOR;
      case GLHCK_SRC_ALPHA:
         return GL_SRC_ALPHA;
      case GLHCK_ONE_MINUS_SRC_ALPHA:
         return GL_ONE_MINUS_SRC_ALPHA;
      case GLHCK_DST_ALPHA:
         return GL_DST_ALPHA;
      case GLHCK_ONE_MINUS_DST_ALPHA:
         return GL_ONE_MINUS_DST_ALPHA;
      case GLHCK_DST_COLOR:
         return GL_DST_COLOR;
      case GLHCK_ONE_MINUS_DST_COLOR:
         return GL_ONE_MINUS_DST_COLOR;
      case GLHCK_SRC_ALPHA_SATURATE:
         return GL_SRC_ALPHA_SATURATE;
      case GLHCK_CONSTANT_COLOR:
         return GL_CONSTANT_COLOR;
      case GLHCK_CONSTANT_ALPHA:
         return GL_CONSTANT_ALPHA;
      case GLHCK_ONE_MINUS_CONSTANT_ALPHA:
         return GL_ONE_MINUS_CONSTANT_ALPHA;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck fbo type to OpenGL fbo type */
GLenum glhFramebufferTypeForGlhckType(glhckFramebufferType type)
{
   switch (type) {
      case GLHCK_FRAMEBUFFER:
         return GL_FRAMEBUFFER;
      case GLHCK_FRAMEBUFFER_READ:
         return GL_READ_FRAMEBUFFER;
      case GLHCK_FRAMEBUFFER_DRAW:
         return GL_DRAW_FRAMEBUFFER;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck fbo attachment constants to OpenGL attachment constants */
GLenum glhAttachmentTypeForGlhckType(glhckFramebufferAttachmentType type)
{
   switch (type) {
      case GLHCK_COLOR_ATTACHMENT0:
         return GL_COLOR_ATTACHMENT0;
      case GLHCK_COLOR_ATTACHMENT1:
         return GL_COLOR_ATTACHMENT1;
      case GLHCK_COLOR_ATTACHMENT2:
         return GL_COLOR_ATTACHMENT2;
      case GLHCK_COLOR_ATTACHMENT3:
         return GL_COLOR_ATTACHMENT3;
      case GLHCK_COLOR_ATTACHMENT4:
         return GL_COLOR_ATTACHMENT4;
      case GLHCK_COLOR_ATTACHMENT5:
         return GL_COLOR_ATTACHMENT5;
      case GLHCK_COLOR_ATTACHMENT6:
         return GL_COLOR_ATTACHMENT6;
      case GLHCK_COLOR_ATTACHMENT7:
         return GL_COLOR_ATTACHMENT7;
      case GLHCK_COLOR_ATTACHMENT8:
         return GL_COLOR_ATTACHMENT8;
      case GLHCK_COLOR_ATTACHMENT9:
         return GL_COLOR_ATTACHMENT9;
      case GLHCK_COLOR_ATTACHMENT10:
         return GL_COLOR_ATTACHMENT10;
      case GLHCK_COLOR_ATTACHMENT11:
         return GL_COLOR_ATTACHMENT11;
      case GLHCK_COLOR_ATTACHMENT12:
         return GL_COLOR_ATTACHMENT12;
      case GLHCK_COLOR_ATTACHMENT13:
         return GL_COLOR_ATTACHMENT13;
      case GLHCK_COLOR_ATTACHMENT14:
         return GL_COLOR_ATTACHMENT14;
      case GLHCK_COLOR_ATTACHMENT15:
         return GL_COLOR_ATTACHMENT15;
      case GLHCK_DEPTH_ATTACHMENT:
         return GL_DEPTH_ATTACHMENT;
      case GLHCK_STENCIL_ATTACHMENT:
         return GL_STENCIL_ATTACHMENT;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck hardware buffer type to OpenGL hw buffer type */
GLenum glhHwBufferTypeForGlhckType(glhckHwBufferType type)
{
   switch (type) {
      case GLHCK_ARRAY_BUFFER:
         return GL_ARRAY_BUFFER;
      case GLHCK_COPY_READ_BUFFER:
         return GL_COPY_READ_BUFFER;
      case GLHCK_COPY_WRITE_BUFFER:
         return GL_COPY_WRITE_BUFFER;
      case GLHCK_ELEMENT_ARRAY_BUFFER:
         return GL_ELEMENT_ARRAY_BUFFER;
      case GLHCK_PIXEL_PACK_BUFFER:
         return GL_PIXEL_PACK_BUFFER;
      case GLHCK_PIXEL_UNPACK_BUFFER:
         return GL_PIXEL_UNPACK_BUFFER;
      case GLHCK_TEXTURE_BUFFER:
         return GL_TEXTURE_BUFFER;
      case GLHCK_TRANSFORM_FEEDBACK_BUFFER:
         return GL_TRANSFORM_FEEDBACK_BUFFER;
      case GLHCK_UNIFORM_BUFFER:
         return GL_UNIFORM_BUFFER;
      case GLHCK_SHADER_STORAGE_BUFFER:
         return GL_SHADER_STORAGE_BUFFER;
      case GLHCK_ATOMIC_COUNTER_BUFFER:
         return GL_ATOMIC_COUNTER_BUFFER;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck hardware buffer store type for OpenGL hw buffer store type */
GLenum glhHwBufferStoreTypeForGlhckType(glhckHwBufferStoreType type)
{
   switch (type) {
      case GLHCK_BUFFER_STREAM_DRAW:
         return GL_STREAM_DRAW;
      case GLHCK_BUFFER_STREAM_READ:
         return GL_STREAM_READ;
      case GLHCK_BUFFER_STREAM_COPY:
         return GL_STREAM_COPY;
      case GLHCK_BUFFER_STATIC_DRAW:
         return GL_STATIC_DRAW;
      case GLHCK_BUFFER_STATIC_READ:
         return GL_STATIC_READ;
      case GLHCK_BUFFER_STATIC_COPY:
         return GL_STATIC_COPY;
      case GLHCK_BUFFER_DYNAMIC_DRAW:
         return GL_DYNAMIC_DRAW;
      case GLHCK_BUFFER_DYNAMIC_READ:
         return GL_DYNAMIC_READ;
      case GLHCK_BUFFER_DYNAMIC_COPY:
         return GL_DYNAMIC_COPY;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck hardware buffer access type for OpenGL access type */
GLenum glhHwBufferAccessTypeForGlhckType(glhckHwBufferAccessType type)
{
   switch (type) {
      case GLHCK_BUFFER_READ_ONLY:
         return GL_READ_ONLY;
      case GLHCK_BUFFER_WRITE_ONLY:
         return GL_WRITE_ONLY;
      case GLHCK_BUFFER_READ_WRITE:
         return GL_READ_WRITE;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck shader type to OpenGL shader type */
GLenum glhShaderTypeForGlhckType(glhckShaderType type)
{
   switch (type) {
      case GLHCK_VERTEX_SHADER:
         return GL_VERTEX_SHADER;
      case GLHCK_FRAGMENT_SHADER:
         return GL_FRAGMENT_SHADER;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map glhck shader constants to OpenGL shader constants */
GLenum glhShaderVariableTypeForGlhckType(_glhckShaderVariableType type)
{
   switch (type) {
      case GLHCK_SHADER_FLOAT:
         return GL_FLOAT;
      case GLHCK_SHADER_FLOAT_VEC2:
         return GL_FLOAT_VEC2;
      case GLHCK_SHADER_FLOAT_VEC3:
         return GL_FLOAT_VEC3;
      case GLHCK_SHADER_FLOAT_VEC4:
         return GL_FLOAT_VEC4;
      case GLHCK_SHADER_DOUBLE:
         return GL_DOUBLE;
      case GLHCK_SHADER_DOUBLE_VEC2:
         return GL_DOUBLE_VEC2;
      case GLHCK_SHADER_DOUBLE_VEC3:
         return GL_DOUBLE_VEC3;
      case GLHCK_SHADER_DOUBLE_VEC4:
         return GL_DOUBLE_VEC4;
      case GLHCK_SHADER_INT:
         return GL_INT;
      case GLHCK_SHADER_INT_VEC2:
         return GL_INT_VEC2;
      case GLHCK_SHADER_INT_VEC3:
         return GL_INT_VEC3;
      case GLHCK_SHADER_INT_VEC4:
         return GL_INT_VEC4;
      case GLHCK_SHADER_UNSIGNED_INT:
         return GL_UNSIGNED_INT;
      case GLHCK_SHADER_UNSIGNED_INT_VEC2:
         return GL_UNSIGNED_INT_VEC2;
      case GLHCK_SHADER_UNSIGNED_INT_VEC3:
         return GL_UNSIGNED_INT_VEC3;
      case GLHCK_SHADER_UNSIGNED_INT_VEC4:
         return GL_UNSIGNED_INT_VEC4;
      case GLHCK_SHADER_BOOL:
         return GL_BOOL;
      case GLHCK_SHADER_BOOL_VEC2:
         return GL_BOOL_VEC2;
      case GLHCK_SHADER_BOOL_VEC3:
         return GL_BOOL_VEC3;
      case GLHCK_SHADER_BOOL_VEC4:
         return GL_BOOL_VEC4;
      case GLHCK_SHADER_FLOAT_MAT2:
         return GL_FLOAT_MAT2;
      case GLHCK_SHADER_FLOAT_MAT3:
         return GL_FLOAT_MAT3;
      case GLHCK_SHADER_FLOAT_MAT4:
         return GL_FLOAT_MAT4;
      case GLHCK_SHADER_FLOAT_MAT2x3:
         return GL_FLOAT_MAT2x3;
      case GLHCK_SHADER_FLOAT_MAT2x4:
         return GL_FLOAT_MAT2x4;
      case GLHCK_SHADER_FLOAT_MAT3x2:
         return GL_FLOAT_MAT3x2;
      case GLHCK_SHADER_FLOAT_MAT3x4:
         return GL_FLOAT_MAT3x4;
      case GLHCK_SHADER_FLOAT_MAT4x2:
         return GL_FLOAT_MAT4x2;
      case GLHCK_SHADER_FLOAT_MAT4x3:
         return GL_FLOAT_MAT4x3;
      case GLHCK_SHADER_DOUBLE_MAT2:
         return GL_DOUBLE_MAT2;
      case GLHCK_SHADER_DOUBLE_MAT3:
         return GL_DOUBLE_MAT3;
      case GLHCK_SHADER_DOUBLE_MAT4:
         return GL_DOUBLE_MAT4;
      case GLHCK_SHADER_DOUBLE_MAT2x3:
         return GL_DOUBLE_MAT2x3;
      case GLHCK_SHADER_DOUBLE_MAT2x4:
         return GL_DOUBLE_MAT2x4;
      case GLHCK_SHADER_DOUBLE_MAT3x2:
         return GL_DOUBLE_MAT3x2;
      case GLHCK_SHADER_DOUBLE_MAT3x4:
         return GL_DOUBLE_MAT3x4;
      case GLHCK_SHADER_DOUBLE_MAT4x2:
         return GL_DOUBLE_MAT4x2;
      case GLHCK_SHADER_DOUBLE_MAT4x3:
         return GL_DOUBLE_MAT4x3;
      case GLHCK_SHADER_SAMPLER_1D:
         return GL_SAMPLER_1D;
      case GLHCK_SHADER_SAMPLER_2D:
         return GL_SAMPLER_2D;
      case GLHCK_SHADER_SAMPLER_3D:
         return GL_SAMPLER_3D;
      case GLHCK_SHADER_SAMPLER_CUBE:
         return GL_SAMPLER_CUBE;
      case GLHCK_SHADER_SAMPLER_1D_SHADOW:
         return GL_SAMPLER_1D_SHADOW;
      case GLHCK_SHADER_SAMPLER_2D_SHADOW:
         return GL_SAMPLER_2D_SHADOW;
      case GLHCK_SHADER_SAMPLER_1D_ARRAY:
         return GL_SAMPLER_1D_ARRAY;
      case GLHCK_SHADER_SAMPLER_2D_ARRAY:
         return GL_SAMPLER_2D_ARRAY;
      case GLHCK_SHADER_SAMPLER_1D_ARRAY_SHADOW:
         return GL_SAMPLER_1D_ARRAY_SHADOW;
      case GLHCK_SHADER_SAMPLER_2D_ARRAY_SHADOW:
         return GL_SAMPLER_2D_ARRAY_SHADOW;
      case GLHCK_SHADER_SAMPLER_2D_MULTISAMPLE:
         return GL_SAMPLER_2D_MULTISAMPLE;
      case GLHCK_SHADER_SAMPLER_2D_MULTISAMPLE_ARRAY:
         return GL_SAMPLER_2D_MULTISAMPLE_ARRAY;
      case GLHCK_SHADER_SAMPLER_CUBE_SHADOW:
         return GL_SAMPLER_CUBE_SHADOW;
      case GLHCK_SHADER_SAMPLER_BUFFER:
         return GL_SAMPLER_BUFFER;
      case GLHCK_SHADER_SAMPLER_2D_RECT:
         return GL_SAMPLER_2D_RECT;
      case GLHCK_SHADER_SAMPLER_2D_RECT_SHADOW:
         return GL_SAMPLER_2D_RECT_SHADOW;
      case GLHCK_SHADER_INT_SAMPLER_1D:
         return GL_INT_SAMPLER_1D;
      case GLHCK_SHADER_INT_SAMPLER_2D:
         return GL_INT_SAMPLER_2D;
      case GLHCK_SHADER_INT_SAMPLER_3D:
         return GL_INT_SAMPLER_3D;
      case GLHCK_SHADER_INT_SAMPLER_CUBE:
         return GL_INT_SAMPLER_CUBE;
      case GLHCK_SHADER_INT_SAMPLER_1D_ARRAY:
         return GL_INT_SAMPLER_1D_ARRAY;
      case GLHCK_SHADER_INT_SAMPLER_2D_ARRAY:
         return GL_INT_SAMPLER_2D_ARRAY;
      case GLHCK_SHADER_INT_SAMPLER_2D_MULTISAMPLE:
         return GL_INT_SAMPLER_2D_MULTISAMPLE;
      case GLHCK_SHADER_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
         return GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
      case GLHCK_SHADER_INT_SAMPLER_BUFFER:
         return GL_INT_SAMPLER_BUFFER;
      case GLHCK_SHADER_INT_SAMPLER_2D_RECT:
         return GL_INT_SAMPLER_2D_RECT;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_1D:
         return GL_UNSIGNED_INT_SAMPLER_1D;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D:
         return GL_UNSIGNED_INT_SAMPLER_2D;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_3D:
         return GL_UNSIGNED_INT_SAMPLER_3D;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_CUBE:
         return GL_UNSIGNED_INT_SAMPLER_CUBE;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_1D_ARRAY:
         return GL_UNSIGNED_INT_SAMPLER_1D_ARRAY;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_ARRAY:
         return GL_UNSIGNED_INT_SAMPLER_2D_ARRAY;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
         return GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
         return GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_BUFFER:
         return GL_UNSIGNED_INT_SAMPLER_BUFFER;
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_RECT:
         return GL_UNSIGNED_INT_SAMPLER_2D_RECT;
      case GLHCK_SHADER_IMAGE_1D:
         return GL_IMAGE_1D;
      case GLHCK_SHADER_IMAGE_2D:
         return GL_IMAGE_2D;
      case GLHCK_SHADER_IMAGE_3D:
         return GL_IMAGE_3D;
      case GLHCK_SHADER_IMAGE_2D_RECT:
         return GL_IMAGE_2D_RECT;
      case GLHCK_SHADER_IMAGE_CUBE:
         return GL_IMAGE_CUBE;
      case GLHCK_SHADER_IMAGE_BUFFER:
         return GL_IMAGE_BUFFER;
      case GLHCK_SHADER_IMAGE_1D_ARRAY:
         return GL_IMAGE_1D_ARRAY;
      case GLHCK_SHADER_IMAGE_2D_ARRAY:
         return GL_IMAGE_2D_ARRAY;
      case GLHCK_SHADER_IMAGE_2D_MULTISAMPLE:
         return GL_IMAGE_2D_MULTISAMPLE;
      case GLHCK_SHADER_IMAGE_2D_MULTISAMPLE_ARRAY:
         return GL_IMAGE_2D_MULTISAMPLE_ARRAY;
     case GLHCK_SHADER_INT_IMAGE_1D:
         return GL_INT_IMAGE_1D;
      case GLHCK_SHADER_INT_IMAGE_2D:
         return GL_INT_IMAGE_2D;
      case GLHCK_SHADER_INT_IMAGE_3D:
         return GL_INT_IMAGE_3D;
      case GLHCK_SHADER_INT_IMAGE_2D_RECT:
         return GL_INT_IMAGE_2D_RECT;
      case GLHCK_SHADER_INT_IMAGE_CUBE:
         return GL_INT_IMAGE_CUBE;
      case GLHCK_SHADER_INT_IMAGE_BUFFER:
         return GL_INT_IMAGE_BUFFER;
      case GLHCK_SHADER_INT_IMAGE_1D_ARRAY:
         return GL_INT_IMAGE_1D_ARRAY;
      case GLHCK_SHADER_INT_IMAGE_2D_ARRAY:
         return GL_INT_IMAGE_2D_ARRAY;
      case GLHCK_SHADER_INT_IMAGE_2D_MULTISAMPLE:
         return GL_INT_IMAGE_2D_MULTISAMPLE;
      case GLHCK_SHADER_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
         return GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_1D:
         return GL_UNSIGNED_INT_IMAGE_1D;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D:
         return GL_UNSIGNED_INT_IMAGE_2D;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_3D:
         return GL_UNSIGNED_INT_IMAGE_3D;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_RECT:
         return GL_UNSIGNED_INT_IMAGE_2D_RECT;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_CUBE:
         return GL_UNSIGNED_INT_IMAGE_CUBE;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_BUFFER:
         return GL_UNSIGNED_INT_IMAGE_BUFFER;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_1D_ARRAY:
         return GL_UNSIGNED_INT_IMAGE_1D_ARRAY;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_ARRAY:
         return GL_UNSIGNED_INT_IMAGE_2D_ARRAY;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
         return GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
         return GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
      case GLHCK_SHADER_UNSIGNED_INT_ATOMIC_COUNTER:
         return GL_UNSIGNED_INT_ATOMIC_COUNTER;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* \brief map OpenGL shader constants to glhck shader constants */
_glhckShaderVariableType glhGlhckShaderVariableTypeForGLType(GLenum type)
{
   switch (type) {
      case GL_FLOAT:
         return GLHCK_SHADER_FLOAT;
      case GL_FLOAT_VEC2:
         return GLHCK_SHADER_FLOAT_VEC2;
      case GL_FLOAT_VEC3:
         return GLHCK_SHADER_FLOAT_VEC3;
      case GL_FLOAT_VEC4:
         return GLHCK_SHADER_FLOAT_VEC4;
      case GL_DOUBLE:
         return GLHCK_SHADER_DOUBLE;
      case GL_DOUBLE_VEC2:
         return GLHCK_SHADER_DOUBLE_VEC2;
      case GL_DOUBLE_VEC3:
         return GLHCK_SHADER_DOUBLE_VEC3;
      case GL_DOUBLE_VEC4:
         return GLHCK_SHADER_DOUBLE_VEC4;
      case GL_INT:
         return GLHCK_SHADER_INT;
      case GL_INT_VEC2:
         return GLHCK_SHADER_INT_VEC2;
      case GL_INT_VEC3:
         return GLHCK_SHADER_INT_VEC3;
      case GL_INT_VEC4:
         return GLHCK_SHADER_INT_VEC4;
      case GL_UNSIGNED_INT:
         return GLHCK_SHADER_UNSIGNED_INT;
      case GL_UNSIGNED_INT_VEC2:
         return GLHCK_SHADER_UNSIGNED_INT_VEC2;
      case GL_UNSIGNED_INT_VEC3:
         return GLHCK_SHADER_UNSIGNED_INT_VEC3;
      case GL_UNSIGNED_INT_VEC4:
         return GLHCK_SHADER_UNSIGNED_INT_VEC4;
      case GL_BOOL:
         return GLHCK_SHADER_BOOL;
      case GL_BOOL_VEC2:
         return GLHCK_SHADER_BOOL_VEC2;
      case GL_BOOL_VEC3:
         return GLHCK_SHADER_BOOL_VEC3;
      case GL_BOOL_VEC4:
         return GLHCK_SHADER_BOOL_VEC4;
      case GL_FLOAT_MAT2:
         return GLHCK_SHADER_FLOAT_MAT2;
      case GL_FLOAT_MAT3:
         return GLHCK_SHADER_FLOAT_MAT3;
      case GL_FLOAT_MAT4:
         return GLHCK_SHADER_FLOAT_MAT4;
      case GL_FLOAT_MAT2x3:
         return GLHCK_SHADER_FLOAT_MAT2x3;
      case GL_FLOAT_MAT2x4:
         return GLHCK_SHADER_FLOAT_MAT2x4;
      case GL_FLOAT_MAT3x2:
         return GLHCK_SHADER_FLOAT_MAT3x2;
      case GL_FLOAT_MAT3x4:
         return GLHCK_SHADER_FLOAT_MAT3x4;
      case GL_FLOAT_MAT4x2:
         return GLHCK_SHADER_FLOAT_MAT4x2;
      case GL_FLOAT_MAT4x3:
         return GLHCK_SHADER_FLOAT_MAT4x3;
      case GL_DOUBLE_MAT2:
         return GLHCK_SHADER_DOUBLE_MAT2;
      case GL_DOUBLE_MAT3:
         return GLHCK_SHADER_DOUBLE_MAT3;
      case GL_DOUBLE_MAT4:
         return GLHCK_SHADER_DOUBLE_MAT4;
      case GL_DOUBLE_MAT2x3:
         return GLHCK_SHADER_DOUBLE_MAT2x3;
      case GL_DOUBLE_MAT2x4:
         return GLHCK_SHADER_DOUBLE_MAT2x4;
      case GL_DOUBLE_MAT3x2:
         return GLHCK_SHADER_DOUBLE_MAT3x2;
      case GL_DOUBLE_MAT3x4:
         return GLHCK_SHADER_DOUBLE_MAT3x4;
      case GL_DOUBLE_MAT4x2:
         return GLHCK_SHADER_DOUBLE_MAT4x2;
      case GL_DOUBLE_MAT4x3:
         return GLHCK_SHADER_DOUBLE_MAT4x3;
      case GL_SAMPLER_1D:
         return GLHCK_SHADER_SAMPLER_1D;
      case GL_SAMPLER_2D:
         return GLHCK_SHADER_SAMPLER_2D;
      case GL_SAMPLER_3D:
         return GLHCK_SHADER_SAMPLER_3D;
      case GL_SAMPLER_CUBE:
         return GLHCK_SHADER_SAMPLER_CUBE;
      case GL_SAMPLER_1D_SHADOW:
         return GLHCK_SHADER_SAMPLER_1D_SHADOW;
      case GL_SAMPLER_2D_SHADOW:
         return GLHCK_SHADER_SAMPLER_2D_SHADOW;
      case GL_SAMPLER_1D_ARRAY:
         return GLHCK_SHADER_SAMPLER_1D_ARRAY;
      case GL_SAMPLER_2D_ARRAY:
         return GLHCK_SHADER_SAMPLER_2D_ARRAY;
      case GL_SAMPLER_1D_ARRAY_SHADOW:
         return GLHCK_SHADER_SAMPLER_1D_ARRAY_SHADOW;
      case GL_SAMPLER_2D_ARRAY_SHADOW:
         return GLHCK_SHADER_SAMPLER_2D_ARRAY_SHADOW;
      case GL_SAMPLER_2D_MULTISAMPLE:
         return GLHCK_SHADER_SAMPLER_2D_MULTISAMPLE;
      case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
         return GLHCK_SHADER_SAMPLER_2D_MULTISAMPLE_ARRAY;
      case GL_SAMPLER_CUBE_SHADOW:
         return GLHCK_SHADER_SAMPLER_CUBE_SHADOW;
      case GL_SAMPLER_BUFFER:
         return GLHCK_SHADER_SAMPLER_BUFFER;
      case GL_SAMPLER_2D_RECT:
         return GLHCK_SHADER_SAMPLER_2D_RECT;
      case GL_SAMPLER_2D_RECT_SHADOW:
         return GLHCK_SHADER_SAMPLER_2D_RECT_SHADOW;
      case GL_INT_SAMPLER_1D:
         return GLHCK_SHADER_INT_SAMPLER_1D;
      case GL_INT_SAMPLER_2D:
         return GLHCK_SHADER_INT_SAMPLER_2D;
      case GL_INT_SAMPLER_3D:
         return GLHCK_SHADER_INT_SAMPLER_3D;
      case GL_INT_SAMPLER_CUBE:
         return GLHCK_SHADER_INT_SAMPLER_CUBE;
      case GL_INT_SAMPLER_1D_ARRAY:
         return GLHCK_SHADER_INT_SAMPLER_1D_ARRAY;
      case GL_INT_SAMPLER_2D_ARRAY:
         return GLHCK_SHADER_INT_SAMPLER_2D_ARRAY;
      case GL_INT_SAMPLER_2D_MULTISAMPLE:
         return GLHCK_SHADER_INT_SAMPLER_2D_MULTISAMPLE;
      case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
         return GLHCK_SHADER_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
      case GL_INT_SAMPLER_BUFFER:
         return GLHCK_SHADER_INT_SAMPLER_BUFFER;
      case GL_INT_SAMPLER_2D_RECT:
         return GLHCK_SHADER_INT_SAMPLER_2D_RECT;
      case GL_UNSIGNED_INT_SAMPLER_1D:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_1D;
      case GL_UNSIGNED_INT_SAMPLER_2D:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D;
      case GL_UNSIGNED_INT_SAMPLER_3D:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_3D;
      case GL_UNSIGNED_INT_SAMPLER_CUBE:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_CUBE;
      case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_1D_ARRAY;
      case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_ARRAY;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
      case GL_UNSIGNED_INT_SAMPLER_BUFFER:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_BUFFER;
      case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
         return GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_RECT;
      case GL_IMAGE_1D:
         return GLHCK_SHADER_IMAGE_1D;
      case GL_IMAGE_2D:
         return GLHCK_SHADER_IMAGE_2D;
      case GL_IMAGE_3D:
         return GLHCK_SHADER_IMAGE_3D;
      case GL_IMAGE_2D_RECT:
         return GLHCK_SHADER_IMAGE_2D_RECT;
      case GL_IMAGE_CUBE:
         return GLHCK_SHADER_IMAGE_CUBE;
      case GL_IMAGE_BUFFER:
         return GLHCK_SHADER_IMAGE_BUFFER;
      case GL_IMAGE_1D_ARRAY:
         return GLHCK_SHADER_IMAGE_1D_ARRAY;
      case GL_IMAGE_2D_ARRAY:
         return GLHCK_SHADER_IMAGE_2D_ARRAY;
      case GL_IMAGE_2D_MULTISAMPLE:
         return GLHCK_SHADER_IMAGE_2D_MULTISAMPLE;
      case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
         return GLHCK_SHADER_IMAGE_2D_MULTISAMPLE_ARRAY;
     case GL_INT_IMAGE_1D:
         return GLHCK_SHADER_INT_IMAGE_1D;
      case GL_INT_IMAGE_2D:
         return GLHCK_SHADER_INT_IMAGE_2D;
      case GL_INT_IMAGE_3D:
         return GLHCK_SHADER_INT_IMAGE_3D;
      case GL_INT_IMAGE_2D_RECT:
         return GLHCK_SHADER_INT_IMAGE_2D_RECT;
      case GL_INT_IMAGE_CUBE:
         return GLHCK_SHADER_INT_IMAGE_CUBE;
      case GL_INT_IMAGE_BUFFER:
         return GLHCK_SHADER_INT_IMAGE_BUFFER;
      case GL_INT_IMAGE_1D_ARRAY:
         return GLHCK_SHADER_INT_IMAGE_1D_ARRAY;
      case GL_INT_IMAGE_2D_ARRAY:
         return GLHCK_SHADER_INT_IMAGE_2D_ARRAY;
      case GL_INT_IMAGE_2D_MULTISAMPLE:
         return GLHCK_SHADER_INT_IMAGE_2D_MULTISAMPLE;
      case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
         return GLHCK_SHADER_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
      case GL_UNSIGNED_INT_IMAGE_1D:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_1D;
      case GL_UNSIGNED_INT_IMAGE_2D:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D;
      case GL_UNSIGNED_INT_IMAGE_3D:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_3D;
      case GL_UNSIGNED_INT_IMAGE_2D_RECT:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_RECT;
      case GL_UNSIGNED_INT_IMAGE_CUBE:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_CUBE;
      case GL_UNSIGNED_INT_IMAGE_BUFFER:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_BUFFER;
      case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_1D_ARRAY;
      case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_ARRAY;
      case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
      case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
         return GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
      case GL_UNSIGNED_INT_ATOMIC_COUNTER:
         return GLHCK_SHADER_UNSIGNED_INT_ATOMIC_COUNTER;
      default:break;
   }
   assert(0 && "BAD ENUM OR NOT IMPLEMENTED");
   return 0;
}

/* return shader variable name for OpenGL shader variable constant */
const GLchar* glhShaderVariableNameForOpenGLConstant(GLenum type)
{
   switch (type) {
      case GL_FLOAT: return "float";
      case GL_FLOAT_VEC2: return "vec2";
      case GL_FLOAT_VEC3: return "vec3";
      case GL_FLOAT_VEC4: return "vec4";
      case GL_DOUBLE: return "double";
      case GL_DOUBLE_VEC2: return "dvec2";
      case GL_DOUBLE_VEC3: return "dvec3";
      case GL_DOUBLE_VEC4: return "dvec4";
      case GL_INT: return "int";
      case GL_INT_VEC2: return "ivec2";
      case GL_INT_VEC3: return "ivec3";
      case GL_INT_VEC4: return "ivec4";
      case GL_UNSIGNED_INT: return "uint";
      case GL_UNSIGNED_INT_VEC2: return "uvec2";
      case GL_UNSIGNED_INT_VEC3: return "uvec3";
      case GL_UNSIGNED_INT_VEC4: return "uvec4";
      case GL_BOOL: return "bool";
      case GL_BOOL_VEC2: return "bvec2";
      case GL_BOOL_VEC3: return "bvec3";
      case GL_BOOL_VEC4: return "bvec4";
      case GL_FLOAT_MAT2: return "mat2";
      case GL_FLOAT_MAT3: return "mat3";
      case GL_FLOAT_MAT4: return "mat4";
      case GL_FLOAT_MAT2x3: return "mat2x3";
      case GL_FLOAT_MAT2x4: return "mat2x4";
      case GL_FLOAT_MAT3x2: return "mat3x2";
      case GL_FLOAT_MAT3x4: return "mat3x4";
      case GL_FLOAT_MAT4x2: return "mat4x2";
      case GL_FLOAT_MAT4x3: return "mat4x3";
      case GL_DOUBLE_MAT2: return "dmat2";
      case GL_DOUBLE_MAT3: return "dmat3";
      case GL_DOUBLE_MAT4: return "dmat4";
      case GL_DOUBLE_MAT2x3: return "dmat2x3";
      case GL_DOUBLE_MAT2x4: return "dmat2x4";
      case GL_DOUBLE_MAT3x2: return "dmat3x2";
      case GL_DOUBLE_MAT3x4: return "dmat3x4";
      case GL_DOUBLE_MAT4x2: return "dmat4x2";
      case GL_DOUBLE_MAT4x3: return "dmat4x3";
      case GL_SAMPLER_1D: return "sampler1D";
      case GL_SAMPLER_2D: return "sampler2D";
      case GL_SAMPLER_3D: return "sampler3D";
      case GL_SAMPLER_CUBE: return "samplerCube";
      case GL_SAMPLER_1D_SHADOW: return "sampler1DShadow";
      case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
      case GL_SAMPLER_1D_ARRAY: return "sampler1DArray";
      case GL_SAMPLER_2D_ARRAY: return "sampler2DArray";
      case GL_SAMPLER_1D_ARRAY_SHADOW: return "sampler1DArrayShadow";
      case GL_SAMPLER_2D_ARRAY_SHADOW: return "sampler2DArrayShadow";
      case GL_SAMPLER_2D_MULTISAMPLE: return "sampler2DMS";
      case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return "sampler2DMSArray";
      case GL_SAMPLER_CUBE_SHADOW: return "samplerCubeShadow";
      case GL_SAMPLER_BUFFER: return "samplerBuffer";
      case GL_SAMPLER_2D_RECT: return "sampler2DRect";
      case GL_SAMPLER_2D_RECT_SHADOW: return "sampler2DRectShadow";
      case GL_INT_SAMPLER_1D: return "isampler1D";
      case GL_INT_SAMPLER_2D: return "isampler2D";
      case GL_INT_SAMPLER_3D: return "isampler3D";
      case GL_INT_SAMPLER_CUBE: return "isamplerCube";
      case GL_INT_SAMPLER_1D_ARRAY: return "isampler1DArray";
      case GL_INT_SAMPLER_2D_ARRAY: return "isampler2DArray";
      case GL_INT_SAMPLER_2D_MULTISAMPLE: return "isampler2DMS";
      case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "isampler2DMSArray";
      case GL_INT_SAMPLER_BUFFER: return "isamplerBuffer";
      case GL_INT_SAMPLER_2D_RECT: return "isampler2DRect";
      case GL_UNSIGNED_INT_SAMPLER_1D: return "usampler1D";
      case GL_UNSIGNED_INT_SAMPLER_2D: return "usampler2D";
      case GL_UNSIGNED_INT_SAMPLER_3D: return "usampler3D";
      case GL_UNSIGNED_INT_SAMPLER_CUBE: return "usamplerCube";
      case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return "usampler2DArray";
      case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return "usampler2DArray";
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return "usampler2DMS";
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "usampler2DMSArray";
      case GL_UNSIGNED_INT_SAMPLER_BUFFER: return "usamplerBuffer";
      case GL_UNSIGNED_INT_SAMPLER_2D_RECT: return "usampler2DRect";
      case GL_IMAGE_1D: return "image1D";
      case GL_IMAGE_2D: return "image2D";
      case GL_IMAGE_3D: return "image3D";
      case GL_IMAGE_2D_RECT: return "image2DRect";
      case GL_IMAGE_CUBE: return "imageCube";
      case GL_IMAGE_BUFFER: return "imageBuffer";
      case GL_IMAGE_1D_ARRAY: return "image1DArray";
      case GL_IMAGE_2D_ARRAY: return "image2DArray";
      case GL_IMAGE_2D_MULTISAMPLE: return "image2DMS";
      case GL_IMAGE_2D_MULTISAMPLE_ARRAY: return "image2DMSArray";
      case GL_INT_IMAGE_1D: return "iimage1D";
      case GL_INT_IMAGE_2D: return "iimage2D";
      case GL_INT_IMAGE_3D: return "iimage3D";
      case GL_INT_IMAGE_2D_RECT: return "iimage2DRect";
      case GL_INT_IMAGE_CUBE: return "iimageCube";
      case GL_INT_IMAGE_BUFFER: return "iimageBuffer";
      case GL_INT_IMAGE_1D_ARRAY: return "iimage1DArray";
      case GL_INT_IMAGE_2D_ARRAY: return "iimage2DArray";
      case GL_INT_IMAGE_2D_MULTISAMPLE: return "iimage2DMS";
      case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return "iimage2DMSArray";
      case GL_UNSIGNED_INT_IMAGE_1D: return "uimage1D";
      case GL_UNSIGNED_INT_IMAGE_2D: return "uimage2D";
      case GL_UNSIGNED_INT_IMAGE_3D: return "uimage3D";
      case GL_UNSIGNED_INT_IMAGE_2D_RECT: return "uimage2DRect";
      case GL_UNSIGNED_INT_IMAGE_CUBE: return "uimageCube";
      case GL_UNSIGNED_INT_IMAGE_BUFFER: return "uimageBuffer";
      case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return "uimage1DArray";
      case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return "uimage2DArray";
      case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return "uimage2DMS";
      case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return "uimage2DMSArray";
      case GL_UNSIGNED_INT_ATOMIC_COUNTER: return "atomic_uint";
      default:break;
   }
   return "unknown";
}

/* return shader variable name for glhck shader variable constant */
const GLchar* glhShaderVariableNameForGlhckConstant(GLenum type) {
   return glhShaderVariableNameForOpenGLConstant(glhShaderVariableTypeForGlhckType(type));
}

/*
 * binding mappings
 */

void glhTextureBind(glhckTextureType type, GLuint object) {
   GL_CALL(glBindTexture(glhTextureTypeForGlhckType(type), object));
}
void glhRenderbufferBind(GLuint object) {
   GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, object));
}
void glhFramebufferBind(glhckFramebufferType type, GLuint object) {
   GL_CALL(glBindFramebuffer(glhFramebufferTypeForGlhckType(type), object));
}

void glhHwBufferBind(glhckHwBufferType type, GLuint object) {
   GL_CALL(glBindBuffer(glhHwBufferTypeForGlhckType(type), object));
}
void glhHwBufferBindBase(glhckHwBufferType type, GLuint index, GLuint object) {
   GL_CALL(glBindBufferBase(glhHwBufferTypeForGlhckType(type), index, object));
}
void glhHwBufferBindRange(glhckHwBufferType type, GLuint index, GLuint object, GLintptr offset, GLsizeiptr size) {
   GL_CALL(glBindBufferRange(glhHwBufferTypeForGlhckType(type), index, object, offset, size));
}
void glhHwBufferCreate(glhckHwBufferType type, GLsizeiptr size, const GLvoid *data, glhckHwBufferStoreType usage) {
   GL_CALL(glBufferData(glhHwBufferTypeForGlhckType(type), size, data, glhHwBufferStoreTypeForGlhckType(usage)));
}
void glhHwBufferFill(glhckHwBufferType type, GLintptr offset, GLsizeiptr size, const GLvoid *data) {
   GL_CALL(glBufferSubData(glhHwBufferTypeForGlhckType(type), offset, size, data));
}
void* glhHwBufferMap(glhckHwBufferType type, glhckHwBufferAccessType access) {
   return glMapBuffer(glhHwBufferTypeForGlhckType(type), glhHwBufferAccessTypeForGlhckType(access));
}
void glhHwBufferUnmap(glhckHwBufferType type) {
   GL_CALL(glUnmapBuffer(glhHwBufferTypeForGlhckType(type)));
}

/*
 * shared OpenGL renderer functions
 */

/* \brief get OpenGL parameters */
void glhGetIntegerv(GLenum pname, int *params)
{
   CALL(1, "%u, %p", pname, params);
   GL_CALL(glGetIntegerv(glhRenderPropertyForGlhckProperty(pname), params));
}

/* \brief clear OpenGL buffers */
void glhClear(GLuint bufferBits)
{
   GLuint glBufferBits = 0;
   TRACE(2);

   if (bufferBits & GLHCK_COLOR_BUFFER)
      glBufferBits |= GL_COLOR_BUFFER_BIT;
   if (bufferBits & GLHCK_DEPTH_BUFFER)
      glBufferBits |= GL_DEPTH_BUFFER_BIT;

   GL_CALL(glClear(glBufferBits));
}

/* \brief set OpenGL clear color */
void glhClearColor(char r, char g, char b, char a)
{
   TRACE(1);
   float fr = (float)r/255, fg = (float)g/255, fb = (float)b/255;
   float fa = (float)a/255;

   GL_CALL(glClearColor(fr, fg, fb, fa));
   GLHCKRD()->clearColor.r = r;
   GLHCKRD()->clearColor.g = g;
   GLHCKRD()->clearColor.b = b;
   GLHCKRD()->clearColor.a = a;
}

/* \brief set cull side */
void glhCullFace(glhckCullFaceType face)
{
   GL_CALL(glCullFace(glhCullFaceTypeForGlhckType(face)));
}

/* \brief get pixels from OpenGL */
void glhBufferGetPixels(GLint x, GLint y, GLsizei width, GLsizei height,
      glhckTextureFormat format, GLvoid *data)
{
   CALL(1, "%d, %d, %d, %d, %d, %p", x, y, width, height, format, data);
   GL_CALL(glReadPixels(x, y, width, height,
            glhTextureFormatForGlhckFormat(format), GL_UNSIGNED_BYTE, data));
}

/* \brief blend func wrapper */
void glhBlendFunc(GLenum blenda, GLenum blendb)
{
   GL_CALL(glBlendFunc(
            glhBlendingModeForGlhckMode(blenda),
            glhBlendingModeForGlhckMode(blendb)));
}

/* \brief create texture from data and upload it to OpenGL */
GLuint glhTextureCreate(glhckTextureType type,
      const GLvoid *buffer, GLsizei size, GLsizei width, GLsizei height, GLsizei depth,
      glhckTextureFormat format, GLuint reuseTextureObject, GLuint flags)
{
   GLuint object;
   glhckTexture *old;
   GLenum glTarget, glFormat;
   CALL(0, "%p, %d, %d, %d, %d, %u, %u", buffer, size,
         width, height, format, reuseTextureObject, flags);

   /* create empty texture */
   if (!(object = reuseTextureObject)) {
      GL_CALL(glGenTextures(1, &object));
   }

   /* fail? */
   if (!object)
      goto _return;

   glTarget = glhTextureTypeForGlhckType(type);
   glFormat = glhTextureFormatForGlhckFormat(format);

   /* store old texture */
   old = GLHCKRD()->texture[type];
   GL_CALL(glhTextureBind(type, object));

   if (flags & GLHCK_TEXTURE_NEAREST) {
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST));
   } else {
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR));
   }

   if (flags & GLHCK_TEXTURE_REPEAT) {
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, GL_REPEAT));
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, GL_REPEAT));
   } else {
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      GL_CALL(glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
   }

   if (_glhckIsCompressedFormat(format)) {
      switch (type) {
         case GLHCK_TEXTURE_1D:
            GL_CALL(glCompressedTexImage1D(glTarget, 0, glFormat, width, 0, size, buffer));
            break;
         case GLHCK_TEXTURE_2D:
            GL_CALL(glCompressedTexImage2D(glTarget, 0, glFormat, width, height, 0, size, buffer));
            break;
         case GLHCK_TEXTURE_3D:
            GL_CALL(glCompressedTexImage3D(glTarget, 0, glFormat, width, height, depth, 0, size, buffer));
            break;
         default:assert(0 && "texture type not supported");break;
      }
   } else {
      switch (type) {
         case GLHCK_TEXTURE_1D:
            GL_CALL(glTexImage1D(glTarget, 0, glFormat, width, 0, glFormat, GL_UNSIGNED_BYTE, buffer));
            break;
         case GLHCK_TEXTURE_2D:
         case GLHCK_TEXTURE_CUBE_MAP:
            GL_CALL(glTexImage2D(glTarget, 0, glFormat, width, height, 0, glFormat, GL_UNSIGNED_BYTE, buffer));
            break;
         case GLHCK_TEXTURE_3D:
            GL_CALL(glTexImage3D(glTarget, 0, glFormat, width, height, depth, 0, glFormat, GL_UNSIGNED_BYTE, buffer));
            break;
         default:assert(0 && "texture type not supported");break;
      }
   }

   /* generate mipmaps */
   glGenerateMipmap(glTarget);

   /* restore old texture */
   if (old) glhckTextureBind(old);
   else glhckTextureUnbind(type);

_return:
   RET(0, "%u", object);
   return object;
}

/* \brief fill texture with data */
void glhTextureFill(glhckTextureType type, GLuint texture, const GLvoid *data, GLsizei size,
      GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, glhckTextureFormat format)
{
   glhckTexture *old;
   GLenum glTarget, glFormat;
   CALL(1, "%u, %p, %d, %d, %d, %d, %d, %d", texture,
         data, size, x, y, width, height, format);

   glTarget = glhTextureTypeForGlhckType(type);
   glFormat = glhTextureFormatForGlhckFormat(format);

   /* store old texture */
   old = GLHCKRD()->texture[type];
   GL_CALL(glhTextureBind(type, texture));

   if (_glhckIsCompressedFormat(format)) {
      switch (type) {
         case GLHCK_TEXTURE_1D:
            GL_CALL(glCompressedTexSubImage1D(glTarget, 0, x, width, glFormat, size, data));
            break;
         case GLHCK_TEXTURE_2D:
            GL_CALL(glCompressedTexSubImage2D(glTarget, 0, x, y, width, height, glFormat, size, data));
            break;
         case GLHCK_TEXTURE_3D:
            GL_CALL(glCompressedTexSubImage3D(glTarget, 0, x, y, z, width, height, depth, glFormat, size, data));
            break;
         default:assert(0 && "texture type not supported");break;
      }
   } else{
      switch (type) {
         case GLHCK_TEXTURE_1D:
            GL_CALL(glTexSubImage1D(glTarget, 0, x, width, glFormat, GL_UNSIGNED_BYTE, data));
            break;
         case GLHCK_TEXTURE_2D:
            GL_CALL(glTexSubImage2D(glTarget, 0, x, y, width, height, glFormat, GL_UNSIGNED_BYTE, data));
            break;
         case GLHCK_TEXTURE_3D:
            GL_CALL(glTexSubImage3D(glTarget, 0, x, y, z, width, height, depth, glFormat, GL_UNSIGNED_BYTE, data));
            break;
         default:assert(0 && "texture type not supported");break;
      }
   }

   /* restore old texture */
   if (old) glhckTextureBind(old);
   else glhckTextureUnbind(type);
}

/* \brief glRenderbufferStorage wrapper */
void glhRenderbufferStorage(GLsizei width, GLsizei height, glhckTextureFormat format)
{
   CALL(0, "%d, %d, %d", width, height, format);
   glRenderbufferStorage(GL_RENDERBUFFER, glhTextureFormatForGlhckFormat(format), width, height);
}

/* \brief glFramebufferTexture wrapper with error checking */
GLint glhFramebufferTexture(glhckFramebufferType framebufferType, glhckTextureType textureType, GLuint texture,
      glhckFramebufferAttachmentType attachment)
{
   GLenum glTarget, glTexTarget, glAttachment;
   CALL(0, "%d, %d, %u, %d", framebufferType, textureType, texture, attachment);

   glTarget     = glhFramebufferTypeForGlhckType(framebufferType);
   glTexTarget  = glhTextureTypeForGlhckType(textureType);
   glAttachment = glhAttachmentTypeForGlhckType(attachment);

   switch (textureType) {
      case GLHCK_TEXTURE_1D:
         GL_CALL(glFramebufferTexture1D(glTarget, glAttachment, glTexTarget, texture, 0));
         break;
      case GLHCK_TEXTURE_2D:
      case GLHCK_TEXTURE_CUBE_MAP:
         GL_CALL(glFramebufferTexture2D(glTarget, glAttachment, glTexTarget, texture, 0));
         break;
      case GLHCK_TEXTURE_3D:
         GL_CALL(glFramebufferTexture3D(glTarget, glAttachment, glTexTarget, texture, 0, 0));
         break;
      default:assert(0 && "texture type not supported");break;
   }

   if (GL_CHECK(glCheckFramebufferStatus(glTarget)) != GL_FRAMEBUFFER_COMPLETE)
      goto fbo_fail;

   RET(0, "%d", RETURN_OK);
   return RETURN_OK;

fbo_fail:
   DEBUG(GLHCK_DBG_ERROR, "Framebuffer is not complete");
   RET(0, "%d", RETURN_FAIL);
   return RETURN_FAIL;
}

/* \brief glFramebufferRenderbuffer wrapper with error checking */
GLint glhFramebufferRenderbuffer(glhckFramebufferType framebufferType, GLuint buffer, glhckFramebufferAttachmentType attachment)
{
   GLenum glTarget, glAttachment;
   CALL(0, "%d, %u, %d", framebufferType, buffer, attachment);

   glTarget     = glhFramebufferTypeForGlhckType(framebufferType);
   glAttachment = glhAttachmentTypeForGlhckType(attachment);
   glFramebufferRenderbuffer(glTarget, glAttachment, GL_RENDERBUFFER, buffer);

   if (GL_CHECK(glCheckFramebufferStatus(glTarget)) != GL_FRAMEBUFFER_COMPLETE)
      goto fbo_fail;

   RET(0, "%d", RETURN_OK);
   return RETURN_OK;

fbo_fail:
   DEBUG(GLHCK_DBG_ERROR, "Framebuffer is not complete");
   RET(0, "%d", RETURN_FAIL);
   return RETURN_FAIL;
}

/* \brief attach uniform buffer object to shader */
GLuint glhProgramAttachUniformBuffer(GLuint program, const GLchar *uboName, GLuint location)
{
   GLuint ubo;
   CALL(0, "%u, %s, %u", program, uboName, location);
   if ((ubo = glGetUniformBlockIndex(program, uboName))) {
      GL_CALL(glUniformBlockBinding(program, ubo, location));
   }
   return ubo;
}

/* \brief create uniform buffer from shader */
_glhckHwBufferShaderUniform* glhProgramUniformBufferList(GLuint program, const GLchar *uboName, GLsizei *uboSize)
{
   GLchar name[255], *uname = NULL;
   GLuint index;
   GLint count, i, *indices = NULL;
   GLsizei length, size;
   GLenum type;
   _glhckHwBufferShaderUniform *uniforms = NULL, *u, *un;
   CALL(0, "%u, %s, %p", program, uboName, uboSize);
   if (uboSize) *uboSize = 0;

    /* get uniform block index */
   index = glGetUniformBlockIndex(program, uboName);
   if (index == GL_INVALID_INDEX) goto fail;

   /* get uniform block size and initialize the hw buffer */
   GL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size));
   if (uboSize) *uboSize = size;

   /* get uniform count for UBO */
   GL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count));
   if (!count) goto no_uniforms;

   /* allocate space for uniform iteration */
   if (!(indices = _glhckMalloc(sizeof(GLuint) * count)))
      goto fail;

   /* get indices for UBO's member uniforms */
   GL_CALL(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices));

   for (i = 0; i < count; ++i) {
      index = (GLuint)indices[i];

      /* get uniform information */
      GL_CALL(glGetActiveUniform(program, index, sizeof(name)-1, &length, &size, &type, name));

      /* cut out [0] for arrays */
      if (!strcmp(name+strlen(name)-3, "[0]"))
         length -= 3;

      /* allocate name */
      if (!(uname = _glhckCalloc(1, length+4)))
         goto fail;

      /* allocate new uniform slot */
      memcpy(uname, name, length);
      for (u = uniforms; u && u->next; u = u->next);
      if (u) u = u->next  = _glhckCalloc(1, sizeof(_glhckHwBufferShaderUniform));
      else   u = uniforms = _glhckCalloc(1, sizeof(_glhckHwBufferShaderUniform));
      if (!u) goto fail;

      /* assign information */
      u->name = uname;
      glGetActiveUniformsiv(program, 1, &index, GL_UNIFORM_OFFSET, &u->offset);
      u->typeName = glhShaderVariableNameForOpenGLConstant(type);
      u->type = glhGlhckShaderVariableTypeForGLType(type);
      u->size = size;
   }

   _glhckFree(indices);
   return uniforms;

fail:
no_uniforms:
   if (uniforms) {
      for (u = uniforms; u; u = un) {
         un = u->next;
         _glhckFree(u);
      }
   }
   IFDO(_glhckFree, indices);
   IFDO(_glhckFree, uname);
   return NULL;
}

/* \brief get attribute list from program */
_glhckShaderAttribute* glhProgramAttributeList(GLuint obj)
{
   GLenum type;
   GLchar name[255];
   GLint count, i;
   GLsizei size, length;
   _glhckShaderAttribute *attributes = NULL, *a, *an;
   CALL(0, "%u", obj);

   /* get attribute count */
   GL_CALL(glGetProgramiv(obj, GL_ACTIVE_ATTRIBUTES, &count));
   if (!count) goto no_attributes;

   for (i = 0; i != count; ++i) {
      /* get attribute information */
      GL_CALL(glGetActiveAttrib(obj, i, sizeof(name)-1, &length, &size, &type, name));

      /* allocate new attribute slot */
      for (a = attributes; a && a->next; a = a->next);
      if (a) a = a->next    = _glhckCalloc(1, sizeof(_glhckShaderAttribute));
      else   a = attributes = _glhckCalloc(1, sizeof(_glhckShaderAttribute));
      if (!a || !(a->name = _glhckMalloc(length+1)))
         goto fail;

      /* assign information */
      memcpy(a->name, name, length+1);
      a->typeName = glhShaderVariableNameForOpenGLConstant(type);
      a->location = glGetAttribLocation(obj, a->name);
      a->type = glhGlhckShaderVariableTypeForGLType(type);
      a->size = size;
   }

   RET(0, "%p", attributes);
   return attributes;

fail:
no_attributes:
   if (attributes) {
      for (a = attributes; a; a = an) {
         an = a->next;
         _glhckFree(a->name);
         _glhckFree(a);
      }
   }
   RET(0, "%p", NULL);
   return NULL;
}

/* \brief get uniform list from program */
_glhckShaderUniform* glhProgramUniformList(GLuint obj)
{
   GLenum type;
   GLchar name[255], *uname = NULL, *tmp;
   GLint count, i, i2;
   GLuint location;
   GLsizei length, size;
   _glhckShaderUniform *uniforms = NULL, *u, *un;
   CALL(0, "%u", obj);

   /* get uniform count */
   GL_CALL(glGetProgramiv(obj, GL_ACTIVE_UNIFORMS, &count));
   if (!count) goto no_uniforms;

   for (i = 0; i != count; ++i) {
      /* get uniform information */
      GL_CALL(glGetActiveUniform(obj, i, sizeof(name)-1, &length, &size, &type, name));

      /* cut out [0] for arrays */
      if (!strcmp(name+strlen(name)-3, "[0]"))
         length -= 3;

      /* allocate name */
      if (!(uname = _glhckCalloc(1, length+4)))
         goto fail;

      /* get uniform location */
      memcpy(uname, name, length);
      location = glGetUniformLocation(obj, uname);
      if (location == UINT_MAX) {
         _glhckFree(uname);
         continue;
      }

      /* allocate new uniform slot */
      for (u = uniforms; u && u->next; u = u->next);
      if (u) u = u->next  = _glhckCalloc(1, sizeof(_glhckShaderUniform));
      else   u = uniforms = _glhckCalloc(1, sizeof(_glhckShaderUniform));
      if (!u) goto fail;

      /* assign information */
      u->name = uname;
      u->typeName = glhShaderVariableNameForOpenGLConstant(type);
      u->location = location;
      u->type = glhGlhckShaderVariableTypeForGLType(type);
      u->size = size;

      /* store for iterating the array slots */
      tmp = uname;

      /* generate uniform bindings for array slots */
      for (i2 = 1; i2 < size; ++i2) {
         if (!(uname = _glhckCalloc(1, length+4)))
            goto fail;

         /* get uniform location */
         snprintf(uname, length+4, "%s[%d]", tmp, i2);
         location = glGetUniformLocation(obj, uname);
         if (location == UINT_MAX) {
            _glhckFree(uname);
            continue;
         }

         for (u = uniforms; u && u->next; u = u->next);
         if (u) u = u->next  = _glhckCalloc(1, sizeof(_glhckShaderUniform));
         else   u = uniforms = _glhckCalloc(1, sizeof(_glhckShaderUniform));
         if (!u) goto fail;

         u->name = uname;
         u->typeName = glhShaderVariableNameForOpenGLConstant(type);
         u->location = location;
         u->type = glhGlhckShaderVariableTypeForGLType(type);
         u->size = size;
      }
   }

   RET(0, "%p", uniforms);
   return uniforms;

fail:
no_uniforms:
   if (uniforms) {
      for (u = uniforms; u; u = un) {
         un = u->next;
         _glhckFree(u);
      }
   }
   IFDO(_glhckFree, uname);
   RET(0, "%p", NULL);
   return NULL;
}

/* \brief set shader uniform */
void glhProgramSetUniform(GLuint obj, _glhckShaderUniform *uniform, GLsizei count, GLvoid *value)
{
   CALL(0, "%u", obj);

   /* automatically figure out the data type */
   switch (uniform->type) {
      case GLHCK_SHADER_FLOAT:
         GL_CALL(glUniform1fv(uniform->location, count, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_VEC2:
         GL_CALL(glUniform2fv(uniform->location, count, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_VEC3:
         GL_CALL(glUniform3fv(uniform->location, count, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_VEC4:
         GL_CALL(glUniform4fv(uniform->location, count, (GLfloat*)value));
         break;
      case GLHCK_SHADER_DOUBLE:
         GL_CALL(glUniform1dv(uniform->location, count, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_VEC2:
         GL_CALL(glUniform2dv(uniform->location, count, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_VEC3:
         GL_CALL(glUniform3dv(uniform->location, count, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_VEC4:
         GL_CALL(glUniform4dv(uniform->location, count, (GLdouble*)value));
         break;
      case GLHCK_SHADER_INT:
      case GLHCK_SHADER_BOOL:
      case GLHCK_SHADER_SAMPLER_1D:
      case GLHCK_SHADER_SAMPLER_2D:
      case GLHCK_SHADER_SAMPLER_3D:
      case GLHCK_SHADER_SAMPLER_CUBE:
      case GLHCK_SHADER_SAMPLER_1D_SHADOW:
      case GLHCK_SHADER_SAMPLER_2D_SHADOW:
      case GLHCK_SHADER_SAMPLER_1D_ARRAY:
      case GLHCK_SHADER_SAMPLER_2D_ARRAY:
      case GLHCK_SHADER_SAMPLER_1D_ARRAY_SHADOW:
      case GLHCK_SHADER_SAMPLER_2D_ARRAY_SHADOW:
      case GLHCK_SHADER_SAMPLER_2D_MULTISAMPLE:
      case GLHCK_SHADER_SAMPLER_2D_MULTISAMPLE_ARRAY:
      case GLHCK_SHADER_SAMPLER_CUBE_SHADOW:
      case GLHCK_SHADER_SAMPLER_BUFFER:
      case GLHCK_SHADER_SAMPLER_2D_RECT:
      case GLHCK_SHADER_SAMPLER_2D_RECT_SHADOW:
      case GLHCK_SHADER_INT_SAMPLER_1D:
      case GLHCK_SHADER_INT_SAMPLER_2D:
      case GLHCK_SHADER_INT_SAMPLER_3D:
      case GLHCK_SHADER_INT_SAMPLER_CUBE:
      case GLHCK_SHADER_INT_SAMPLER_1D_ARRAY:
      case GLHCK_SHADER_INT_SAMPLER_2D_ARRAY:
      case GLHCK_SHADER_INT_SAMPLER_2D_MULTISAMPLE:
      case GLHCK_SHADER_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
      case GLHCK_SHADER_INT_SAMPLER_BUFFER:
      case GLHCK_SHADER_INT_SAMPLER_2D_RECT:
      case GLHCK_SHADER_IMAGE_1D:
      case GLHCK_SHADER_IMAGE_2D:
      case GLHCK_SHADER_IMAGE_3D:
      case GLHCK_SHADER_IMAGE_2D_RECT:
      case GLHCK_SHADER_IMAGE_CUBE:
      case GLHCK_SHADER_IMAGE_BUFFER:
      case GLHCK_SHADER_IMAGE_1D_ARRAY:
      case GLHCK_SHADER_IMAGE_2D_ARRAY:
      case GLHCK_SHADER_IMAGE_2D_MULTISAMPLE:
      case GLHCK_SHADER_IMAGE_2D_MULTISAMPLE_ARRAY:
      case GLHCK_SHADER_INT_IMAGE_1D:
      case GLHCK_SHADER_INT_IMAGE_2D:
      case GLHCK_SHADER_INT_IMAGE_3D:
      case GLHCK_SHADER_INT_IMAGE_2D_RECT:
      case GLHCK_SHADER_INT_IMAGE_CUBE:
      case GLHCK_SHADER_INT_IMAGE_BUFFER:
      case GLHCK_SHADER_INT_IMAGE_1D_ARRAY:
      case GLHCK_SHADER_INT_IMAGE_2D_ARRAY:
      case GLHCK_SHADER_INT_IMAGE_2D_MULTISAMPLE:
      case GLHCK_SHADER_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
         GL_CALL(glUniform1iv(uniform->location, count, (GLint*)value));
         break;
      case GLHCK_SHADER_INT_VEC2:
      case GLHCK_SHADER_BOOL_VEC2:
         GL_CALL(glUniform2iv(uniform->location, count, (GLint*)value));
         break;
      case GLHCK_SHADER_INT_VEC3:
      case GLHCK_SHADER_BOOL_VEC3:
         GL_CALL(glUniform3iv(uniform->location, count, (GLint*)value));
         break;
      case GLHCK_SHADER_INT_VEC4:
      case GLHCK_SHADER_BOOL_VEC4:
         GL_CALL(glUniform4iv(uniform->location, count, (GLint*)value));
         break;
      case GLHCK_SHADER_UNSIGNED_INT:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_1D:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_3D:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_CUBE:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_1D_ARRAY:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_ARRAY:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_BUFFER:
      case GLHCK_SHADER_UNSIGNED_INT_SAMPLER_2D_RECT:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_1D:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_3D:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_RECT:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_CUBE:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_BUFFER:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_1D_ARRAY:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_ARRAY:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
      case GLHCK_SHADER_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
      case GLHCK_SHADER_UNSIGNED_INT_ATOMIC_COUNTER:
         GL_CALL(glUniform1uiv(uniform->location, count, (GLuint*)value));
         break;
      case GLHCK_SHADER_UNSIGNED_INT_VEC2:
         GL_CALL(glUniform2uiv(uniform->location, count, (GLuint*)value));
         break;
      case GLHCK_SHADER_UNSIGNED_INT_VEC3:
         GL_CALL(glUniform3uiv(uniform->location, count, (GLuint*)value));
         break;
      case GLHCK_SHADER_UNSIGNED_INT_VEC4:
         GL_CALL(glUniform4uiv(uniform->location, count, (GLuint*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT2:
         GL_CALL(glUniformMatrix2fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT3:
         GL_CALL(glUniformMatrix3fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT4:
         GL_CALL(glUniformMatrix4fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT2x3:
         GL_CALL(glUniformMatrix2x3fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT2x4:
         GL_CALL(glUniformMatrix2x4fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT3x2:
         GL_CALL(glUniformMatrix3x2fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT3x4:
         GL_CALL(glUniformMatrix3x4fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT4x2:
         GL_CALL(glUniformMatrix4x2fv(uniform->location, count, 0, (GLfloat*)value));
         break;
      case GLHCK_SHADER_FLOAT_MAT4x3:
         GL_CALL(glUniformMatrix4x3fv(uniform->location, count, 0, (GLfloat*)value));
         break;
#if 0
      case GLHCK_SHADER_DOUBLE_MAT2:
         GL_CALL(glUniformMatrix2dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT3:
         GL_CALL(glUniformMatrix3dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT4:
         GL_CALL(glUniformMatrix4dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT2x3:
         GL_CALL(glUniformMatrix2x3dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT2x4:
         GL_CALL(glUniformMatrix2x4dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT3x2:
         GL_CALL(glUniformMatrix3x2dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT3x4:
         GL_CALL(glUniformMatrix3x4dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT4x2:
         GL_CALL(glUniformMatrix4x2dv(uniform->location, count, 0, (GLdouble*)value));
         break;
      case GLHCK_SHADER_DOUBLE_MAT4x3:
         GL_CALL(glUniformMatrix4x3dv(uniform->location, count, 0, (GLdouble*)value));
         break;
#endif
      default:
         DEBUG(GLHCK_DBG_ERROR, "uniform type not implemented.");
         break;
   }
}

/* helper macro for passing indices to OpenGL */
#define indicesToOpenGL(iprec, tunion) \
   GL_CALL(glDrawElements(glGeometryType, geometry->indexCount, iprec, &geometry->indices.tunion[0]))

/* \brief draw interleaved geometry */
inline void glhGeometryRender(const glhckGeometry *geometry, glhckGeometryType type)
{
   GLenum glGeometryType;
   // printf("%s (%d)\n", glhckIndexTypeString(geometry->indexType), geometry->indexCount);

   glGeometryType = glhGeometryTypeForGlhckType(type);
   if (geometry->indexType != GLHCK_INDEX_NONE) {
      switch (geometry->indexType) {
         case GLHCK_INDEX_BYTE:
            indicesToOpenGL(GL_UNSIGNED_BYTE, ivb);
            break;

         case GLHCK_INDEX_SHORT:
            indicesToOpenGL(GL_UNSIGNED_SHORT, ivs);
            break;

         case GLHCK_INDEX_INTEGER:
            indicesToOpenGL(GL_UNSIGNED_INT, ivi);
            break;

         default:
            break;
      }
   } else {
      GL_CALL(glDrawArrays(glGeometryType, 0, geometry->vertexCount));
   }
}

/* the helper is no longer needed */
#undef indicesToOpenGL

/* vim: set ts=8 sw=3 tw=0 :*/
