#ifndef __glhck_h__
#define __glhck_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* for default typedefs */
#include <kazmath/kazmath.h> /* glhck needs kazmath */
#include <kazmath/vec4.h> /* is not include by kazmath.h */

/* System specific defines, stolen from glfw.
 * Thanks for the nice library guys. */

#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__))
#  define _WIN32
#endif /* _WIN32 */

/* In order for extension support to be portable, we need to define an
 * OpenGL function call method. We use the keyword APIENTRY, which is
 * defined for Win32. (Note: Windows also needs this for <GL/gl.h>)
 */
#ifndef APIENTRY
#  ifdef _WIN32
#     define APIENTRY __stdcall
#  else
#     define APIENTRY
#  endif
#endif /* APIENTRY */

/* The following three defines are here solely to make some Windows-based
 * <GL/gl.h> files happy. Theoretically we could include <windows.h>, but
 * it has the major drawback of severely polluting our namespace.
 */

/* Under Windows, we need WINGDIAPI defined */
#if !defined(WINGDIAPI) && defined(_WIN32)
#  if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__POCC__)
/* Microsoft Visual C++, Borland C++ Builder and Pelles C */
#     define WINGDIAPI __declspec(dllimport)
#  elif defined(__LCC__)
/* LCC-Win32 */
#     define WINGDIAPI __stdcall
#  else
/* Others (e.g. MinGW, Cygwin) */
#     define WINGDIAPI extern
#  endif
#     define GLHCK_WINGDIAPI_DEFINED
#endif /* WINGDIAPI */

/* Some <GL/glu.h> files also need CALLBACK defined */
#if !defined(CALLBACK) && defined(_WIN32)
#  if defined(_MSC_VER)
/* Microsoft Visual C++ */
#     if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#        define CALLBACK __stdcall
#     else
#     define CALLBACK
#  endif
#  else
/* Other Windows compilers */
#     define CALLBACK __stdcall
#  endif
#     define GLHCK_CALLBACK_DEFINED
#endif /* CALLBACK */

/* Microsoft Visual C++, Borland C++ and Pelles C <GL*glu.h> needs wchar_t */
#if defined(_WIN32) && (defined(_MSC_VER) || defined(__BORLANDC__) || defined(__POCC__)) && !defined(_WCHAR_T_DEFINED)
   typedef unsigned short wchar_t;
#  define _WCHAR_T_DEFINED
#endif /* _WCHAR_T_DEFINED */

/* GLHCK specific defines */

#if defined(GLHCK_DLL) && defined(_GLHCK_BUILD_DLL)
#  error "You must not have both GLHCK_DLL and _GLHCK_BUILD_DLL defined"
#endif

#if defined(_WIN32) && defined(_GLHCK_BUILD_DLL)
/* We are building a Win32 DLL */
#  define GLHCKAPI __declspec(dllexport)
#elif defined(_WIN32) && defined(GLHCK_DLL)
/* We are calling a Win32 DLL */
#  if defined(__LCC__)
#     define GLHCKAPI extern
#  else
#     define GLHCKAPI __declspec(dllimport)
#  endif
#else
/* We are either building/calling a static lib or we are non-win32 */
#  define GLHCKAPI
#endif

/***
 * public api
 ***/

/* debugging level */
typedef enum glhckDebugLevel {
   GLHCK_DBG_ERROR,
   GLHCK_DBG_WARNING,
   GLHCK_DBG_CRAP,
} glhckDebugLevel;

/* render type */
typedef enum glhckRenderType {
   GLHCK_RENDER_AUTO,
   GLHCK_RENDER_OPENGL,
   GLHCK_RENDER_OPENGL_FIXED_PIPELINE,
   GLHCK_RENDER_GLES1,
   GLHCK_RENDER_GLES2,
   GLHCK_RENDER_STUB,
} glhckRenderType;

/* driver type */
typedef enum glhckDriverType {
   GLHCK_DRIVER_NONE,
   GLHCK_DRIVER_NVIDIA,
   GLHCK_DRIVER_ATI,
   GLHCK_DRIVER_MESA,
   GLHCK_DRIVER_OTHER,
} glhckDriverType;

/* renderer properties */
typedef enum glhckRenderProperty {
   GLHCK_MAX_TEXTURE_SIZE,
   GLHCK_MAX_CUBE_MAP_TEXTURE_SIZE,
   GLHCK_MAX_VERTEX_ATTRIBS,
   GLHCK_MAX_VERTEX_UNIFORM_VECTORS,
   GLHCK_MAX_VARYING_VECTORS,
   GLHCK_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
   GLHCK_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
   GLHCK_MAX_TEXTURE_IMAGE_UNITS,
   GLHCK_MAX_FRAGMENT_UNIFORM_VECTORS,
   GLHCK_MAX_RENDERBUFFER_SIZE,
   GLHCK_MAX_VIEWPORT_DIMS,
} glhckRenderProperty;

/* clear bits */
typedef enum glhckClearBufferBits {
   GLHCK_CLEAR_BIT_NONE,
   GLHCK_COLOR_BUFFER,
   GLHCK_DEPTH_BUFFER,
} glhckClearBufferBits;

/* cull side face */
typedef enum glhckCullFaceType {
   GLHCK_CULL_FRONT,
   GLHCK_CULL_BACK,
} glhckCullFaceType;

/* render pass bits */
typedef enum glhckRenderPassFlag {
   GLHCK_PASS_NOTHING      = 0,
   GLHCK_PASS_DEPTH        = 1,
   GLHCK_PASS_CULL         = 2,
   GLHCK_PASS_BLEND        = 4,
   GLHCK_PASS_TEXTURE      = 8,
   GLHCK_PASS_OBB          = 16,
   GLHCK_PASS_AABB         = 32,
   GLHCK_PASS_WIREFRAME    = 64,
   GLHCK_PASS_LIGHTING     = 128,
   GLHCK_PASS_DEFAULTS     = 256,
} glhckRenderPassFlag;

/* geometry type */
typedef enum glhckGeometryType {
   GLHCK_POINTS,
   GLHCK_LINES,
   GLHCK_LINE_LOOP,
   GLHCK_LINE_STRIP,
   GLHCK_TRIANGLES,
   GLHCK_TRIANGLE_STRIP,
} glhckGeometryType;

/* framebuffer types */
typedef enum glhckFramebufferTarget {
   GLHCK_FRAMEBUFFER,
   GLHCK_FRAMEBUFFER_READ,
   GLHCK_FRAMEBUFFER_DRAW,
   GLHCK_FRAMEBUFFER_TYPE_LAST,
} glhckFramebufferTarget;

/* framebuffer attachment types */
typedef enum glhckFramebufferAttachmentType {
   GLHCK_COLOR_ATTACHMENT0,
   GLHCK_COLOR_ATTACHMENT1,
   GLHCK_COLOR_ATTACHMENT2,
   GLHCK_COLOR_ATTACHMENT3,
   GLHCK_COLOR_ATTACHMENT4,
   GLHCK_COLOR_ATTACHMENT5,
   GLHCK_COLOR_ATTACHMENT6,
   GLHCK_COLOR_ATTACHMENT7,
   GLHCK_COLOR_ATTACHMENT8,
   GLHCK_COLOR_ATTACHMENT9,
   GLHCK_COLOR_ATTACHMENT10,
   GLHCK_COLOR_ATTACHMENT11,
   GLHCK_COLOR_ATTACHMENT12,
   GLHCK_COLOR_ATTACHMENT13,
   GLHCK_COLOR_ATTACHMENT14,
   GLHCK_COLOR_ATTACHMENT15,
   GLHCK_DEPTH_ATTACHMENT,
   GLHCK_STENCIL_ATTACHMENT,
} glhckFramebufferAttachmentType;

/* hardware buffer type */
typedef enum glhckHwBufferTarget {
   GLHCK_ARRAY_BUFFER,
   GLHCK_COPY_READ_BUFFER,
   GLHCK_COPY_WRITE_BUFFER,
   GLHCK_ELEMENT_ARRAY_BUFFER,
   GLHCK_PIXEL_PACK_BUFFER,
   GLHCK_PIXEL_UNPACK_BUFFER,
   GLHCK_TEXTURE_BUFFER,
   GLHCK_TRANSFORM_FEEDBACK_BUFFER,
   GLHCK_UNIFORM_BUFFER,
   GLHCK_SHADER_STORAGE_BUFFER,
   GLHCK_ATOMIC_COUNTER_BUFFER,
   GLHCK_HWBUFFER_TYPE_LAST,
} glhckHwBufferTarget;

/* hardware buffer storage type */
typedef enum glhckHwBufferStoreType {
   GLHCK_BUFFER_STREAM_DRAW,
   GLHCK_BUFFER_STREAM_READ,
   GLHCK_BUFFER_STREAM_COPY,
   GLHCK_BUFFER_STATIC_DRAW,
   GLHCK_BUFFER_STATIC_READ,
   GLHCK_BUFFER_STATIC_COPY,
   GLHCK_BUFFER_DYNAMIC_DRAW,
   GLHCK_BUFFER_DYNAMIC_READ,
   GLHCK_BUFFER_DYNAMIC_COPY,
} glhckHwBufferStoreType;

/* hardware buffer access type */
typedef enum glhckHwBufferAccessType {
   GLHCK_BUFFER_READ_ONLY,
   GLHCK_BUFFER_WRITE_ONLY,
   GLHCK_BUFFER_READ_WRITE,
} glhckHwBufferAccessType;

/* shader type */
typedef enum glhckShaderType {
   GLHCK_VERTEX_SHADER,
   GLHCK_FRAGMENT_SHADER,
} glhckShaderType;

/* projection type */
typedef enum glhckProjectionType {
   GLHCK_PROJECTION_PERSPECTIVE,
   GLHCK_PROJECTION_ORTHOGRAPHIC,
} glhckProjectionType;

/* frustum planes */
typedef enum glhckFrustumPlane {
   GLHCK_FRUSTUM_PLANE_LEFT,
   GLHCK_FRUSTUM_PLANE_RIGHT,
   GLHCK_FRUSTUM_PLANE_BOTTOM,
   GLHCK_FRUSTUM_PLANE_TOP,
   GLHCK_FRUSTUM_PLANE_NEAR,
   GLHCK_FRUSTUM_PLANE_FAR,
   GLHCK_FRUSTUM_PLANE_LAST
} glhckFrustumPlane;

/* frustum corners */
typedef enum glhckFrustumCorner {
   GLHCK_FRUSTUM_CORNER_BOTTOM_LEFT,
   GLHCK_FRUSTUM_CORNER_BOTTOM_RIGHT,
   GLHCK_FRUSTUM_CORNER_TOP_RIGHT,
   GLHCK_FRUSTUM_CORNER_TOP_LEFT,
   GLHCK_FRUSTUM_CORNER_LAST
} glhckFrustumCorner;

/* frustum struct */
typedef struct glhckFrustum {
   kmPlane planes[GLHCK_FRUSTUM_PLANE_LAST];
   kmVec3 nearCorners[GLHCK_FRUSTUM_CORNER_LAST];
   kmVec3 farCorners[GLHCK_FRUSTUM_CORNER_LAST];
} glhckFrustum;

/* parent affection flags */
typedef enum glhckObjectAffectionFlags {
   GLHCK_AFFECT_NONE          = 0,
   GLHCK_AFFECT_TRANSLATION   = 1,
   GLHCK_AFFECT_ROTATION      = 2,
   GLHCK_AFFECT_SCALING       = 4,
} glhckObjectAffectionFlags;

/* texture wrap modes */
typedef enum glhckTextureWrap {
   GLHCK_WRAP_REPEAT,
   GLHCK_WRAP_MIRRORED_REPEAT,
   GLHCK_WRAP_CLAMP_TO_EDGE,
   GLHCK_WRAP_CLAMP_TO_BORDER,
} glhckTextureWrap;

/* texture filter modes */
typedef enum glhckTextureFilter {
   GLHCK_FILTER_NEAREST,
   GLHCK_FILTER_LINEAR,
   GLHCK_FILTER_NEAREST_MIPMAP_NEAREST,
   GLHCK_FILTER_LINEAR_MIPMAP_NEAREST,
   GLHCK_FILTER_NEAREST_MIPMAP_LINEAR,
   GLHCK_FILTER_LINEAR_MIPMAP_LINEAR,
} glhckTextureFilter;

/* texture compression type */
typedef enum glhckTextureCompression {
   GLHCK_COMPRESSION_NONE,
   GLHCK_COMPRESSION_DXT,
} glhckTextureCompression;

/* texture compare mode */
typedef enum glhckTextureCompareMode {
   GLHCK_COMPARE_NONE,
   GLHCK_COMPARE_REF_TO_TEXTURE
} glhckTextureCompareMode;

/* compare func */
typedef enum glhckCompareFunc {
   GLHCK_COMPARE_LEQUAL,
   GLHCK_COMPARE_GEQUAL,
   GLHCK_COMPARE_LESS,
   GLHCK_COMPARE_GREATER,
   GLHCK_COMPARE_EQUAL,
   GLHCK_COMPARE_NOTEQUAL,
   GLHCK_COMPARE_ALWAYS,
   GLHCK_COMPARE_NEVER,
} glhckCompareFunc;

/* texture types */
typedef enum glhckTextureTarget {
   GLHCK_TEXTURE_1D,
   GLHCK_TEXTURE_2D,
   GLHCK_TEXTURE_3D,
   GLHCK_TEXTURE_CUBE_MAP,
   GLHCK_TEXTURE_TYPE_LAST,
} glhckTextureTarget;

/* texture formats */
typedef enum glhckTextureFormat {
   GLHCK_RED,
   GLHCK_RG,
   GLHCK_ALPHA,
   GLHCK_LUMINANCE,
   GLHCK_LUMINANCE_ALPHA,
   GLHCK_RGB,
   GLHCK_BGR,
   GLHCK_RGBA,
   GLHCK_BGRA,

   GLHCK_DEPTH_COMPONENT,
   GLHCK_DEPTH_COMPONENT16,
   GLHCK_DEPTH_COMPONENT24,
   GLHCK_DEPTH_COMPONENT32,
   GLHCK_DEPTH_STENCIL,

   GLHCK_COMPRESSED_RGB_DXT1,
   GLHCK_COMPRESSED_RGBA_DXT5,
} glhckTextureFormat;

/* data formats */
typedef enum glhckDataType {
   GLHCK_DATA_COMPRESSED,
   GLHCK_DATA_UNSIGNED_BYTE,
   GLHCK_DATA_BYTE,
   GLHCK_DATA_UNSIGNED_SHORT,
   GLHCK_DATA_SHORT,
   GLHCK_DATA_UNSIGNED_INT,
   GLHCK_DATA_INT,
   GLHCK_DATA_FLOAT,
   GLHCK_DATA_UNSIGNED_BYTE_3_3_2,
   GLHCK_DATA_UNSIGNED_BYTE_2_3_3_REV,
   GLHCK_DATA_UNSIGNED_SHORT_5_6_5,
   GLHCK_DATA_UNSIGNED_SHORT_5_6_5_REV,
   GLHCK_DATA_UNSIGNED_SHORT_4_4_4_4,
   GLHCK_DATA_UNISNGED_SHORT_4_4_4_4_REV,
   GLHCK_DATA_UNISGNED_SHORT_5_5_5_1,
   GLHCK_DATA_UNSIGNED_SHORT_1_5_5_5_REV,
   GLHCK_DATA_UNSIGNED_INT_8_8_8_8,
   GLHCK_DATA_UNSIGNED_INT_8_8_8_8_REV,
   GLHCK_DATA_UNSIGNED_INT_10_10_10_2,
   GLHCK_DATA_UNSIGNED_INT_2_10_10_10_REV,
} glhckDataType;

/* texture parameters struct */
typedef struct glhckTextureParameters {
   float minLod, maxLod, biasLod;
   int baseLevel, maxLevel;
   glhckTextureWrap wrapS, wrapT, wrapR;
   glhckTextureFilter minFilter, magFilter;
   glhckTextureCompareMode compareMode;
   glhckCompareFunc compareFunc;
   glhckTextureCompression compression;
   char mipmap;
} glhckTextureParameters;

/* material flags */
typedef enum glhckMaterialFlags {
   GLHCK_MATERIAL_NONE      = 0,
   GLHCK_MATERIAL_DRAW_AABB = 1,
   GLHCK_MATERIAL_DRAW_OBB  = 2,
   GLHCK_MATERIAL_WIREFRAME = 4,
   GLHCK_MATERIAL_COLOR     = 8,
   GLHCK_MATERIAL_CULL      = 16,
   GLHCK_MATERIAL_DEPTH     = 32,
   GLHCK_MATERIAL_LIGHTING  = 64,
} glhckMaterialFlags;

/* blending modes */
typedef enum glhckBlendingMode
{
   GLHCK_ZERO,
   GLHCK_ONE,
   GLHCK_SRC_COLOR,
   GLHCK_ONE_MINUS_SRC_COLOR,
   GLHCK_SRC_ALPHA,
   GLHCK_ONE_MINUS_SRC_ALPHA,
   GLHCK_DST_ALPHA,
   GLHCK_ONE_MINUS_DST_ALPHA,
   GLHCK_DST_COLOR,
   GLHCK_ONE_MINUS_DST_COLOR,
   GLHCK_SRC_ALPHA_SATURATE,
   GLHCK_CONSTANT_COLOR,
   GLHCK_CONSTANT_ALPHA,
   GLHCK_ONE_MINUS_CONSTANT_ALPHA,
} glhckBlendingMode;

/* model import flags */
typedef enum glhckModelFlags {
   GLHCK_MODEL_NONE     = 0,
   GLHCK_MODEL_ANIMATED = 1,
   GLHCK_MODEL_JOIN     = 2,
} glhckModelFlags;

/* common datatypes */
typedef struct glhckRect {
   float x, y, w, h;
} glhckRect;

/* vertex datatypes */
typedef struct glhckColorb {
   unsigned char r, g, b, a;
} glhckColorb;

typedef struct glhckVector3b {
   char x, y, z;
} glhckVector3b;

typedef struct glhckVector2b {
   char x, y;
} glhckVector2b;

typedef struct glhckVector3s {
   short x, y, z;
} glhckVector3s;

typedef struct glhckVector2s {
   short x, y;
} glhckVector2s;

typedef struct glhckVector3f {
   float x, y, z;
} glhckVector3f;

typedef struct glhckVector2f {
   float x, y;
} glhckVector2f;

/* internal vertex data structures,
 * if you modify the object's vertexdata directly
 * make sure you cast to the right data type. */
typedef struct glhckVertexData3b {
   glhckVector3b vertex;
   glhckVector3s normal;
   glhckVector2s coord;
   glhckColorb color;
} glhckVertexData3b;

typedef struct glhckVertexData2b {
   glhckVector2b vertex;
   glhckVector3s normal;
   glhckVector2s coord;
   glhckColorb color;
} glhckVertexData2b;

typedef struct glhckVertexData3s {
   glhckVector3s vertex;
   glhckVector3s normal;
   glhckVector2s coord;
   glhckColorb color;
} glhckVertexData3s;

typedef struct glhckVertexData2s {
   glhckVector2s vertex;
   glhckVector3s normal;
   glhckVector2s coord;
   glhckColorb color;
} glhckVertexData2s;

typedef struct glhckVertexData3fs {
   glhckVector3f vertex;
   glhckVector3s normal;
   glhckVector2s coord;
   glhckColorb color;
} glhckVertexData3fs;

typedef struct glhckVertexData2fs {
   glhckVector2f vertex;
   glhckVector3s normal;
   glhckVector2s coord;
   glhckColorb color;
} glhckVertexData2fs;

typedef struct glhckVertexData3f {
   glhckVector3f vertex;
   glhckVector3f normal;
   glhckVector2f coord;
   glhckColorb color;
} glhckVertexData3f;

typedef struct glhckVertexData2f {
   glhckVector2f vertex;
   glhckVector3f normal;
   glhckVector2f coord;
   glhckColorb color;
} glhckVertexData2f;

/* feed glhck the highest precision */
typedef glhckVertexData3f glhckImportVertexData;

typedef union glhckVertexData {
      glhckVertexData3b *v3b;
      glhckVertexData2b *v2b;
      glhckVertexData3s *v3s;
      glhckVertexData2s *v2s;
      glhckVertexData3fs *v3fs;
      glhckVertexData2fs *v2fs;
      glhckVertexData3f *v3f;
      glhckVertexData2f *v2f;
      void *any;
} glhckVertexData;

typedef unsigned char glhckIndexb;
typedef unsigned short glhckIndexs;
typedef unsigned int glhckIndexi;

/* feed glhck the highest precision */
typedef glhckIndexi glhckImportIndexData;

typedef union glhckIndexData {
      glhckIndexb *ivb;
      glhckIndexs *ivs;
      glhckIndexi *ivi;
      void *any;
} glhckIndexData;

/* geometry type enums */
typedef enum glhckGeometryVertexType {
   GLHCK_VERTEX_NONE,
   GLHCK_VERTEX_V3B,
   GLHCK_VERTEX_V2B,
   GLHCK_VERTEX_V3S,
   GLHCK_VERTEX_V2S,
   GLHCK_VERTEX_V3FS,
   GLHCK_VERTEX_V2FS,
   GLHCK_VERTEX_V3F,
   GLHCK_VERTEX_V2F
} glhckGeometryVertexType;

typedef enum glhckGeometryIndexType {
   GLHCK_INDEX_NONE,
   GLHCK_INDEX_BYTE,
   GLHCK_INDEX_SHORT,
   GLHCK_INDEX_INTEGER
} glhckGeometryIndexType;

/* teture coordinate transformation */
typedef struct glhckCoordTransform {
   glhckRect transform;
   short degrees;
} glhckCoordTransform;

/* datatype that presents object's geometry */
typedef struct glhckGeometry {
   /* geometry transformation needed
    * after precision conversion */
   glhckVector3f bias;
   glhckVector3f scale;

   /* vertices */
   glhckVertexData vertices;

   /* indices */
   glhckIndexData indices;

   /* transformed coordinates */
   glhckCoordTransform *transformedCoordinates;

   /* counts for vertices && indices */
   int vertexCount, indexCount;

   /* transformed texture range
    * the texture matrix is scaled with
    * 1.0f/textureRange of geometry before
    * sending to OpenGL */
   int textureRange;

   /* vertex && indices types */
   glhckGeometryVertexType vertexType;
   glhckGeometryIndexType indexType;

   /* geometry type (triangles, triangle strip, etc..) */
   glhckGeometryType type;
} glhckGeometry;

/* vector animation key */
typedef struct glhckAnimationVectorKey {
   kmVec3 vector;
   float time;
} glhckAnimationVectorKey;

/* quaternion animation key */
typedef struct glhckAnimationQuaternionKey {
   kmQuaternion quaternion;
   float time;
} glhckAnimationQuaternionKey;

/* datatype that presents bone's vertex weight */
typedef struct glhckVertexWeight {
   float weight;
   glhckIndexi vertexIndex;
} glhckVertexWeight;

/* typedefs for better typing */
typedef struct _glhckTexture           glhckTexture;
typedef struct _glhckAtlas             glhckAtlas;
typedef struct _glhckRenderbuffer      glhckRenderbuffer;
typedef struct _glhckFramebuffer       glhckFramebuffer;
typedef struct _glhckObject            glhckObject;
typedef struct _glhckBone              glhckBone;
typedef struct _glhckAnimation         glhckAnimation;
typedef struct _glhckAnimationNode     glhckAnimationNode;
typedef struct _glhckAnimator          glhckAnimator;
typedef struct _glhckText              glhckText;
typedef struct _glhckCamera            glhckCamera;
typedef struct _glhckLight             glhckLight;
typedef struct _glhckHwBuffer          glhckHwBuffer;
typedef struct _glhckShader            glhckShader;
typedef struct __GLHCKcontext          glhckContext;

typedef void (*glhckDebugHookFunc)(const char *file, int line, const char *function, glhckDebugLevel level, const char *str);

/* init && terminate */
GLHCKAPI int glhckInitialized(void);
GLHCKAPI void glhckContextSet(glhckContext *ctx);
GLHCKAPI glhckContext* glhckContextGet(void);
GLHCKAPI glhckContext* glhckContextCreate(int argc, char **argv);
GLHCKAPI void glhckContextTerminate(void);
GLHCKAPI void glhckMassacreWorld(void);
GLHCKAPI void glhckLogColor(char color);
GLHCKAPI void glhckSetGlobalPrecision(glhckGeometryIndexType itype, glhckGeometryVertexType vtype);
GLHCKAPI void glhckGetGlobalPrecision(glhckGeometryIndexType *itype, glhckGeometryVertexType *vtype);

/* display */
GLHCKAPI int glhckDisplayCreate(int width, int height, glhckRenderType renderType);
GLHCKAPI void glhckDisplayClose(void);
GLHCKAPI void glhckDisplayResize(int width, int height);

/* rendering */
GLHCKAPI const char* glhckRenderName(void);
GLHCKAPI glhckDriverType glhckRenderGetDriver(void);
GLHCKAPI void glhckRenderResize(int width, int height);
GLHCKAPI void glhckRenderViewport(int x, int y, int width, int height);
GLHCKAPI void glhckRenderPassFlags(unsigned int bits);
GLHCKAPI void glhckRenderTime(float time);
GLHCKAPI void glhckRenderClearColor(const glhckColorb *color);
GLHCKAPI void glhckRenderClearColorb(char r, char g, char b, char a);
GLHCKAPI const glhckColorb* glhckRenderGetClearColor(void);
GLHCKAPI void glhckRenderClear(unsigned int bufferBits);
GLHCKAPI void glhckRenderBlendFunc(glhckBlendingMode blenda, glhckBlendingMode blendb);
GLHCKAPI void glhckRenderGetBlendFunc(glhckBlendingMode *blenda, glhckBlendingMode *blendb);
GLHCKAPI void glhckRenderCullFace(glhckCullFaceType face);
GLHCKAPI glhckCullFaceType glhckRenderGetCullFace(void);
GLHCKAPI void glhckRenderPassShader(glhckShader *shader);
GLHCKAPI glhckShader* glhckRenderGetShader(void);
GLHCKAPI void glhckRenderProjection(kmMat4 const* mat);
GLHCKAPI void glhckRenderProjectionOnly(const kmMat4 *mat);
GLHCKAPI const kmMat4* glhckRenderGetProjection(void);
GLHCKAPI void glhckRenderView(kmMat4 const* mat);
GLHCKAPI const kmMat4* glhckRenderGetView(void);
GLHCKAPI void glhckRenderWorldPosition(const kmVec3 *position);
GLHCKAPI void glhckRender(void);

/* frustum */
GLHCKAPI void glhckFrustumBuild(glhckFrustum *frustum, const kmMat4 *mvp);
GLHCKAPI void glhckFrustumRender(glhckFrustum *frustum);
GLHCKAPI int glhckFrustumContainsPoint(const glhckFrustum *frustum, const kmVec3 *point);
GLHCKAPI kmScalar glhckFrustumContainsSphere(const glhckFrustum *frustum, const kmVec3 *point, kmScalar radius);
GLHCKAPI int glhckFrustumContainsAABB(const glhckFrustum *frustum, const kmAABB *aabb);

/* cameras */
GLHCKAPI glhckCamera* glhckCameraNew(void);
GLHCKAPI glhckCamera* glhckCameraRef(glhckCamera *camera);
GLHCKAPI unsigned int glhckCameraFree(glhckCamera *camera);
GLHCKAPI void glhckCameraUpdate(glhckCamera *camera);
GLHCKAPI void glhckCameraReset(glhckCamera *camera);
GLHCKAPI void glhckCameraProjection(glhckCamera *camera, const glhckProjectionType projectionType);
GLHCKAPI glhckFrustum* glhckCameraGetFrustum(glhckCamera *camera);
GLHCKAPI const kmMat4* glhckCameraGetViewMatrix(const glhckCamera *camera);
GLHCKAPI const kmMat4* glhckCameraGetProjectionMatrix(const glhckCamera *camera);
GLHCKAPI const kmMat4* glhckCameraGetVPMatrix(const glhckCamera *camera);
GLHCKAPI void glhckCameraUpVector(glhckCamera *camera, const kmVec3 *upVector);
GLHCKAPI void glhckCameraFov(glhckCamera *camera, const kmScalar fov);
GLHCKAPI void glhckCameraRange(glhckCamera *camera, const kmScalar near, const kmScalar far);
GLHCKAPI void glhckCameraViewport(glhckCamera *camera, const glhckRect *viewport);
GLHCKAPI void glhckCameraViewportf(glhckCamera *camera, int x, int y, int w, int h);
GLHCKAPI glhckObject* glhckCameraGetObject(const glhckCamera *camera);

/* objects */
GLHCKAPI glhckObject* glhckObjectNew(void);
GLHCKAPI glhckObject* glhckObjectCopy(const glhckObject *src);
GLHCKAPI glhckObject* glhckObjectRef(glhckObject *object);
GLHCKAPI unsigned int glhckObjectFree(glhckObject *object);

GLHCKAPI int glhckObjectIsRoot(const glhckObject *object);
GLHCKAPI void glhckObjectMakeRoot(glhckObject *object, int root);
GLHCKAPI unsigned char glhckObjectGetParentAffection(const glhckObject *object);
GLHCKAPI void glhckObjectParentAffection(glhckObject *object, unsigned char affectionFlags);
GLHCKAPI glhckObject* glhckObjectParent(glhckObject *object);
GLHCKAPI glhckObject** glhckObjectChildren(glhckObject *object, unsigned int *memb);
GLHCKAPI void glhckObjectAddChild(glhckObject *object, glhckObject *child);
GLHCKAPI void glhckObjectRemoveChild(glhckObject *object, glhckObject *child);
GLHCKAPI void glhckObjectRemoveChildren(glhckObject *object);
GLHCKAPI void glhckObjectRemoveFromParent(glhckObject *object);
GLHCKAPI void glhckObjectTexture(glhckObject *object, glhckTexture *texture);
GLHCKAPI glhckTexture* glhckObjectGetTexture(const glhckObject *object);
GLHCKAPI void glhckObjectShader(glhckObject *object, glhckShader *shader);
GLHCKAPI glhckShader* glhckObjectGetShader(const glhckObject *object);
GLHCKAPI void glhckObjectDraw(glhckObject *object);
GLHCKAPI void glhckObjectRender(glhckObject *object);
GLHCKAPI int glhckObjectInsertVertices(glhckObject *object, glhckGeometryVertexType type, const glhckImportVertexData *vertices, int memb);
GLHCKAPI int glhckObjectInsertIndices(glhckObject *object, glhckGeometryIndexType type, const glhckImportIndexData *indices, int memb);
GLHCKAPI int glhckObjectInsertBones(glhckObject *object, glhckBone **bones, unsigned int memb);
GLHCKAPI glhckBone** glhckObjectBones(glhckObject *object, unsigned int *memb);
GLHCKAPI int glhckObjectInsertAnimations(glhckObject *object, glhckAnimation **animations, unsigned int memb);
GLHCKAPI glhckAnimation** glhckObjectAnimations(glhckObject *object, unsigned int *memb);
GLHCKAPI void glhckObjectUpdate(glhckObject *object);

/* material */
GLHCKAPI const glhckColorb* glhckObjectGetColor(const glhckObject *object);
GLHCKAPI void glhckObjectColor(glhckObject *object, const glhckColorb *color);
GLHCKAPI void glhckObjectColorb(glhckObject *object, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
GLHCKAPI unsigned int glhckObjectGetMaterialFlags(const glhckObject *object);
GLHCKAPI void glhckObjectMaterialFlags(glhckObject *object, unsigned int flags);
GLHCKAPI void glhckObjectMaterialBlend(glhckObject *object, glhckBlendingMode blenda, glhckBlendingMode blendb);

/* object control */
GLHCKAPI const kmAABB* glhckObjectGetOBB(const glhckObject *object);
GLHCKAPI const kmAABB* glhckObjectGetAABB(const glhckObject *object);
GLHCKAPI const kmMat4* glhckObjectGetMatrix(glhckObject *object);
GLHCKAPI const kmVec3* glhckObjectGetPosition(const glhckObject *object);
GLHCKAPI void glhckObjectPosition(glhckObject *object, const kmVec3 *position);
GLHCKAPI void glhckObjectPositionf(glhckObject *object, const kmScalar x, const kmScalar y, const kmScalar z);
GLHCKAPI void glhckObjectMove(glhckObject *object, const kmVec3 *move);
GLHCKAPI void glhckObjectMovef(glhckObject *object, const kmScalar x, const kmScalar y, const kmScalar z);
GLHCKAPI const kmVec3* glhckObjectGetRotation(const glhckObject *object);
GLHCKAPI void glhckObjectRotation(glhckObject *object, const kmVec3 *rotate);
GLHCKAPI void glhckObjectRotationf(glhckObject *object, const kmScalar x, const kmScalar y, const kmScalar z);
GLHCKAPI void glhckObjectRotate(glhckObject *object, const kmVec3 *rotate);
GLHCKAPI void glhckObjectRotatef(glhckObject *object, const kmScalar x, const kmScalar y, const kmScalar z);
GLHCKAPI const kmVec3* glhckObjectGetTarget(const glhckObject *object);
GLHCKAPI void glhckObjectTarget(glhckObject *object, const kmVec3 *target);
GLHCKAPI void glhckObjectTargetf(glhckObject *object, const kmScalar x, const kmScalar y, const kmScalar z);
GLHCKAPI const kmVec3* glhckObjectGetScale(const glhckObject *object);
GLHCKAPI void glhckObjectScale(glhckObject *object, const kmVec3 *scale);
GLHCKAPI void glhckObjectScalef(glhckObject *object, const kmScalar x, const kmScalar y, const kmScalar z);

/* object geometry */
GLHCKAPI glhckGeometry* glhckObjectNewGeometry(glhckObject *object);
GLHCKAPI glhckGeometry* glhckObjectGetGeometry(const glhckObject *object);

/* pre-defined geometry */
GLHCKAPI glhckObject* glhckModelNew(const char *file, kmScalar size, unsigned int importFlags);
GLHCKAPI glhckObject* glhckModelNewEx(const char *file, kmScalar size, unsigned int importFlags,
      glhckGeometryIndexType itype, glhckGeometryVertexType vtype);
GLHCKAPI glhckObject* glhckCubeNew(kmScalar size);
GLHCKAPI glhckObject* glhckCubeNewEx(kmScalar size, glhckGeometryIndexType itype, glhckGeometryVertexType vtype);
GLHCKAPI glhckObject* glhckPlaneNew(kmScalar width, kmScalar height);
GLHCKAPI glhckObject* glhckPlaneNewEx(kmScalar width, kmScalar height, glhckGeometryIndexType itype, glhckGeometryVertexType vtype);
GLHCKAPI glhckObject* glhckSpriteNew(glhckTexture* texture, kmScalar width, kmScalar height);
GLHCKAPI glhckObject* glhckSpriteNewFromFile(const char *file, kmScalar width, kmScalar height, unsigned int importFlags, const glhckTextureParameters *params);

/* bones */
GLHCKAPI glhckBone* glhckBoneNew(void);
GLHCKAPI glhckBone* glhckBoneRef(glhckBone *object);
GLHCKAPI unsigned int glhckBoneFree(glhckBone *object);
GLHCKAPI void glhckBoneName(glhckBone *object, const char *name);
GLHCKAPI const char* glhckBoneGetName(glhckBone *object);
GLHCKAPI int glhckBoneInsertWeights(glhckBone *object, const glhckVertexWeight *weights, unsigned int memb);
GLHCKAPI const glhckVertexWeight* glhckBoneWeights(glhckBone *object, unsigned int *memb);
GLHCKAPI void glhckBoneParentBone(glhckBone *object, glhckBone *parentBone);
GLHCKAPI glhckBone* glhckBoneGetParentBone(glhckBone *object);
GLHCKAPI void glhckBoneOffsetMatrix(glhckBone *object, const kmMat4 *offsetMatrix);
GLHCKAPI const kmMat4* glhckBoneGetOffsetMatrix(glhckBone *object);
GLHCKAPI void glhckBoneTransformationMatrix(glhckBone *object, const kmMat4 *transformationMatrix);
GLHCKAPI const kmMat4* glhckBoneGetTransformationMatrix(glhckBone *object);
GLHCKAPI const kmMat4* glhckBoneGetTransformedMatrix(glhckBone *object);

/* animation nodes */
GLHCKAPI glhckAnimationNode* glhckAnimationNodeNew(void);
GLHCKAPI glhckAnimationNode* glhckAnimationNodeRef(glhckAnimationNode *object);
GLHCKAPI unsigned int glhckAnimationNodeFree(glhckAnimationNode *object);
GLHCKAPI void glhckAnimationNodeBoneName(glhckAnimationNode *object, const char *name);
GLHCKAPI const char* glhckAnimationNodeGetBoneName(glhckAnimationNode *object);
GLHCKAPI int glhckAnimationNodeInsertTranslations(glhckAnimationNode *object, const glhckAnimationVectorKey *keys, unsigned int memb);
GLHCKAPI const glhckAnimationVectorKey* glhckAnimationNodeTranslations(glhckAnimationNode *object, unsigned int *memb);
GLHCKAPI int glhckAnimationNodeInsertRotations(glhckAnimationNode *object, const glhckAnimationQuaternionKey *keys, unsigned int memb);
GLHCKAPI const glhckAnimationQuaternionKey* glhckAnimationNodeRotations(glhckAnimationNode *object, unsigned int *memb);
GLHCKAPI int glhckAnimationNodeInsertScalings(glhckAnimationNode *object, const glhckAnimationVectorKey *keys, unsigned int memb);
GLHCKAPI const glhckAnimationVectorKey* glhckAnimationNodeScalings(glhckAnimationNode *object, unsigned int *memb);

/* animations */
GLHCKAPI glhckAnimation* glhckAnimationNew(void);
GLHCKAPI glhckAnimation* glhckAnimationRef(glhckAnimation *object);
GLHCKAPI unsigned int glhckAnimationFree(glhckAnimation *object);
GLHCKAPI void glhckAnimationName(glhckAnimation *object, const char *name);
GLHCKAPI const char* glhckAnimationGetName(glhckAnimation *object);
GLHCKAPI void glhckAnimationTicksPerSecond(glhckAnimation *object, float ticksPerSecond);
GLHCKAPI float glhckAnimationGetTicksPerSecond(glhckAnimation *object);
GLHCKAPI void glhckAnimationDuration(glhckAnimation *object, float duration);
GLHCKAPI float glhckAnimationGetDuration(glhckAnimation *object);
GLHCKAPI int glhckAnimationInsertNodes(glhckAnimation *object, glhckAnimationNode **nodes, unsigned int memb);
GLHCKAPI glhckAnimationNode** glhckAnimationNodes(glhckAnimation *object, unsigned int *memb);

/* animator */
GLHCKAPI glhckAnimator* glhckAnimatorNew(void);
GLHCKAPI glhckAnimator* glhckAnimatorRef(glhckAnimator *object);
GLHCKAPI unsigned int glhckAnimatorFree(glhckAnimator *object);
GLHCKAPI void glhckAnimatorAnimation(glhckAnimator *object, glhckAnimation *animation);
GLHCKAPI int glhckAnimatorInsertBones(glhckAnimator *object, glhckBone **bones, unsigned int memb);
GLHCKAPI glhckBone** glhckAnimatorBones(glhckAnimator *object, unsigned int *memb);
GLHCKAPI void glhckAnimatorTransform(glhckAnimator *object, glhckObject *gobject);
GLHCKAPI void glhckAnimatorUpdate(glhckAnimator *object, float playTime);

/* text */
GLHCKAPI glhckText* glhckTextNew(int cachew, int cacheh);
GLHCKAPI glhckText* glhckTextRef(glhckText *text);
GLHCKAPI unsigned int glhckTextFree(glhckText *text);
GLHCKAPI void glhckTextGetMetrics(glhckText *text, unsigned int font_id, float size, float *ascender, float *descender, float *lineh);
GLHCKAPI void glhckTextGetMinMax(glhckText *text, unsigned int font_id, float size, const char *s, kmVec2 *min, kmVec2 *max);
GLHCKAPI void glhckTextColor(glhckText *text, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
GLHCKAPI const glhckColorb* glhckGetTextColor(glhckText *text);
GLHCKAPI unsigned int glhckTextNewFontFromMemory(glhckText *text, const void *data, size_t size);
GLHCKAPI unsigned int glhckTextNewFont(glhckText *text, const char *file);
GLHCKAPI unsigned int glhckTextNewFontFromBitmap(glhckText *text, const char *file, int ascent, int descent, int line_gap);
GLHCKAPI void glhckTextNewGlyph(glhckText *text, unsigned int font_id, const char *s,
      short size, short base, int x, int y, int w, int h,
      float xoff, float yoff, float xadvance);
GLHCKAPI void glhckTextStash(glhckText *text, unsigned int font_id, float size, float x, float y, const char *s, float *dx);
GLHCKAPI void glhckTextClear(glhckText *text);
GLHCKAPI void glhckTextRender(glhckText *text);
GLHCKAPI void glhckTextShader(glhckText *text, glhckShader *shader);
GLHCKAPI glhckShader* glhckTextGetShader(const glhckText *text);
GLHCKAPI glhckTexture* glhckTextRTT(glhckText *text, unsigned int font_id, float size, const char *s, const glhckTextureParameters *params);
GLHCKAPI glhckObject* glhckTextPlane(glhckText *text, unsigned int font_id, float size, const char *s, const glhckTextureParameters *params);

/* textures */
GLHCKAPI glhckTexture* glhckTextureNew(const char *file, unsigned int importFlags, const glhckTextureParameters *params);
GLHCKAPI glhckTexture* glhckTextureCopy(glhckTexture *src);
GLHCKAPI glhckTexture* glhckTextureRef(glhckTexture *texture);
GLHCKAPI unsigned int glhckTextureFree(glhckTexture *texture);
GLHCKAPI const void* glhckTextureGetData(glhckTexture *texture, int *size);
GLHCKAPI void glhckTextureGetInformation(glhckTexture *texture, glhckTextureTarget *target, int *width, int *height, int *depth, int *border, glhckTextureFormat *format, glhckDataType *type);
GLHCKAPI int glhckTextureCreate(glhckTexture *texture, glhckTextureTarget target, int level, int width, int height, int depth, int border, glhckTextureFormat format, glhckDataType type, int size, const void *data);
GLHCKAPI int glhckTextureRecreate(glhckTexture *texture, glhckTextureFormat format, glhckDataType type, int size, const void *data);
GLHCKAPI void glhckTextureFill(glhckTexture *texture, int level, int x, int y, int z, int width, int height, int depth, glhckTextureFormat format, glhckDataType type, int size, const void *data);
GLHCKAPI void glhckTextureParameter(glhckTexture *texture, const glhckTextureParameters *params);
GLHCKAPI const glhckTextureParameters* glhckTextureDefaultParameters(void);
GLHCKAPI int glhckTextureSave(glhckTexture *texture, const char *path);
GLHCKAPI glhckTexture* glhckTextureCurrentForTarget(glhckTextureTarget target);
GLHCKAPI void glhckTextureActive(unsigned int index);
GLHCKAPI void glhckTextureBind(glhckTexture *texture);
GLHCKAPI void glhckTextureUnbind(glhckTextureTarget target);

/* texture atlases */
GLHCKAPI glhckAtlas* glhckAtlasNew(void);
GLHCKAPI unsigned int glhckAtlasFree(glhckAtlas *atlas);
GLHCKAPI int glhckAtlasInsertTexture(glhckAtlas *atlas, glhckTexture *texture);
GLHCKAPI glhckTexture* glhckAtlasGetTexture(glhckAtlas *atlas);
GLHCKAPI int glhckAtlasPack(glhckAtlas *atlas, const int power_of_two, const int border);
GLHCKAPI glhckTexture* glhckAtlasGetTextureByIndex(const glhckAtlas *atlas, unsigned short index);
GLHCKAPI int glhckAtlasGetTransform(const glhckAtlas *atlas, glhckTexture *texture, glhckRect *transformed, short *degrees);
GLHCKAPI int glhckAtlasTransformCoordinates(const glhckAtlas *atlas, glhckTexture *texture, const kmVec2 *in, kmVec2 *out);

/* light objects */
GLHCKAPI glhckLight* glhckLightNew(void);
GLHCKAPI glhckLight* glhckLightRef(glhckLight *object);
GLHCKAPI unsigned int glhckLightFree(glhckLight *object);
GLHCKAPI glhckObject* glhckLightGetObject(const glhckLight *object);
GLHCKAPI void glhckLightBind(glhckLight *object);
GLHCKAPI void glhckLightBeginProjectionWithCamera(glhckLight *object, glhckCamera *camera);
GLHCKAPI void glhckLightEndProjectionWithCamera(glhckLight *object, glhckCamera *camera);
GLHCKAPI void glhckLightColor(glhckLight *object, const glhckColorb *color);
GLHCKAPI void glhckLightColorb(glhckLight *object, char r, char g, char b, char a);
GLHCKAPI void glhckLightPointLightFactor(glhckLight *object, kmScalar factor);
GLHCKAPI kmScalar glhckLightGetPointLightFactor(glhckLight *object);
GLHCKAPI void glhckLightAtten(glhckLight *object, const kmVec3 *atten);
GLHCKAPI void glhckLightAttenf(glhckLight *object, kmScalar constant, kmScalar linear, kmScalar quadratic);
GLHCKAPI const kmVec3* glhckLightGetAtten(glhckLight *object);
GLHCKAPI void glhckLightCutout(glhckLight *object, const kmVec2 *cutout);
GLHCKAPI void glhckLightCutoutf(glhckLight *object, kmScalar outer, kmScalar inner);
GLHCKAPI const kmVec2* glhckLightGetCutout(glhckLight *object);

/* renderbuffer objects */
GLHCKAPI glhckRenderbuffer* glhckRenderbufferNew(int width, int height, glhckTextureFormat format);
GLHCKAPI glhckRenderbuffer* glhckRenderbufferRef(glhckRenderbuffer *object);
GLHCKAPI unsigned int glhckRenderbufferFree(glhckRenderbuffer *object);
GLHCKAPI void glhckRenderbufferBind(glhckRenderbuffer *object);

/* framebuffer objects */
GLHCKAPI glhckFramebuffer* glhckFramebufferNew(glhckFramebufferTarget target);
GLHCKAPI glhckFramebuffer* glhckFramebufferRef(glhckFramebuffer *object);
GLHCKAPI unsigned int glhckFramebufferFree(glhckFramebuffer *object);
GLHCKAPI glhckFramebuffer* glhckFramebufferCurrentForTarget(glhckFramebufferTarget target);
GLHCKAPI void glhckFramebufferBind(glhckFramebuffer *object);
GLHCKAPI void glhckFramebufferUnbind(glhckFramebufferTarget target);
GLHCKAPI void glhckFramebufferBegin(glhckFramebuffer *object);
GLHCKAPI void glhckFramebufferEnd(glhckFramebuffer *object);
GLHCKAPI void glhckFramebufferRect(glhckFramebuffer *object, glhckRect *rect);
GLHCKAPI void glhckFramebufferRecti(glhckFramebuffer *object, int x, int y, int w, int h);
GLHCKAPI int glhckFramebufferAttachTexture(glhckFramebuffer *object, glhckTexture *texture, glhckFramebufferAttachmentType attachment);
GLHCKAPI int glhckFramebufferFillTexture(glhckFramebuffer *object, glhckTexture *texture);
GLHCKAPI int glhckFramebufferAttachRenderbuffer(glhckFramebuffer *object, glhckRenderbuffer *buffer,
      glhckFramebufferAttachmentType attachment);

/* hardware buffer objects */
GLHCKAPI glhckHwBuffer* glhckHwBufferNew(void);
GLHCKAPI glhckHwBuffer* glhckHwBufferRef(glhckHwBuffer *object);
GLHCKAPI unsigned int glhckHwBufferFree(glhckHwBuffer *object);
GLHCKAPI glhckHwBuffer* glhckHwBufferCurrentForTarget(glhckHwBufferTarget target);
GLHCKAPI void glhckHwBufferBind(glhckHwBuffer *object);
GLHCKAPI void glhckHwBufferUnbind(glhckHwBufferTarget target);
GLHCKAPI glhckHwBufferTarget glhckHwBufferGetType(glhckHwBuffer *object);
GLHCKAPI glhckHwBufferStoreType glhckHwBufferGetStoreType(glhckHwBuffer *object);
GLHCKAPI void glhckHwBufferCreate(glhckHwBuffer *object, glhckHwBufferTarget target, int size, const void *data, glhckHwBufferStoreType usage);
GLHCKAPI void glhckHwBufferBindBase(glhckHwBuffer *object, unsigned int index);
GLHCKAPI void glhckHwBufferBindRange(glhckHwBuffer *object, unsigned int index, int offset, int size);
GLHCKAPI void glhckHwBufferFill(glhckHwBuffer *object, int offset, int size, const void *data);
GLHCKAPI void* glhckHwBufferMap(glhckHwBuffer *object, glhckHwBufferAccessType access);
GLHCKAPI void glhckHwBufferUnmap(glhckHwBuffer *object);

/* uniform buffer specific */
GLHCKAPI int glhckHwBufferCreateUniformBufferFromShader(glhckHwBuffer *object, glhckShader *shader, const char *uboName, glhckHwBufferStoreType usage);
GLHCKAPI int glhckHwBufferAppendInformationFromShader(glhckHwBuffer *object, glhckShader *shader);
GLHCKAPI void glhckHwBufferFillUniform(glhckHwBuffer *object, const char *uniform, int size, const void *data);

/* shaders */
GLHCKAPI unsigned int glhckCompileShaderObject(glhckShaderType type, const char *effectKey, const char *contentsFromMemory);
GLHCKAPI void glhckDeleteShaderObject(unsigned int shaderObject);
GLHCKAPI glhckShader* glhckShaderNew(const char *vertexEffect, const char *fragmentEffect, const char *contentsFromMemory);
GLHCKAPI glhckShader* glhckShaderNewWithShaderObjects(unsigned int vertexShader, unsigned int fragmentShader);
GLHCKAPI glhckShader* glhckShaderRef(glhckShader *object);
GLHCKAPI unsigned int glhckShaderFree(glhckShader *object);
GLHCKAPI void glhckShaderBind(glhckShader *object);
GLHCKAPI void glhckShaderUniform(glhckShader *object, const char *uniform, int count, void *value);
GLHCKAPI int glhckShaderAttachHwBuffer(glhckShader *object, glhckHwBuffer *buffer, const char *name, unsigned int index);

/* trace && debug */
GLHCKAPI void glhckDebugHook(glhckDebugHookFunc func);
GLHCKAPI void glhckMemoryGraph(void);
GLHCKAPI void glhckRenderPrintObjectQueue(void);
GLHCKAPI void glhckRenderPrintTextureQueue(void);

/* vertexdata geometry */
GLHCKAPI size_t glhckIndexTypeElementSize(glhckGeometryIndexType type);
GLHCKAPI glhckIndexi glhckIndexTypeMaxPrecision(glhckGeometryIndexType type);
GLHCKAPI const char* glhckIndexTypeString(glhckGeometryIndexType type);
GLHCKAPI int glhckIndexTypeWithinRange(unsigned int value, glhckGeometryIndexType type);
GLHCKAPI glhckGeometryIndexType glhckIndexTypeForValue(unsigned int value);

GLHCKAPI size_t glhckVertexTypeElementSize(glhckGeometryVertexType type);
GLHCKAPI float glhckVertexTypeMaxPrecision(glhckGeometryVertexType type);
GLHCKAPI const char* glhckVertexTypeString(glhckGeometryVertexType type);
GLHCKAPI glhckGeometryVertexType glhckVertexTypeGetV2Counterpart(glhckGeometryVertexType type);
GLHCKAPI int glhckVertexTypeWithinRange(float value, glhckGeometryVertexType type);
GLHCKAPI glhckGeometryVertexType glhckVertexTypeForSize(kmScalar width, kmScalar height);
GLHCKAPI int glhckVertexTypeHasNormal(glhckGeometryVertexType type);
GLHCKAPI int glhckVertexTypeHasColor(glhckGeometryVertexType type);

GLHCKAPI glhckIndexi glhckGeometryGetVertexIndexForIndex(glhckGeometry *geometry, glhckIndexi ix);
GLHCKAPI void glhckGeometrySetVertexIndexForIndex(glhckGeometry *object, glhckIndexi ix, glhckIndexi vertexIndex);
GLHCKAPI void glhckGeometryGetVertexDataForIndex(
      glhckGeometry *geometry, glhckIndexi ix,
      glhckVector3f *vertex, glhckVector3f *normal,
      glhckVector2f *coord, glhckColorb *color);
GLHCKAPI void glhckGeometrySetVertexDataForIndex(
      glhckGeometry *geometry, glhckIndexi ix,
      glhckVector3f *vertex, glhckVector3f *normal,
      glhckVector2f *coord, glhckColorb *color);
GLHCKAPI void glhckGeometryTransformCoordinates(glhckGeometry *geometry, const glhckRect *transformed, short degrees);
GLHCKAPI void glhckGeometryCalculateBB(glhckGeometry *geometry, kmAABB *bb);
GLHCKAPI int glhckGeometryInsertVertices(glhckGeometry *geometry, glhckGeometryVertexType type, const void *data, int memb);
GLHCKAPI int glhckGeometryInsertIndices(glhckGeometry *geometry, glhckGeometryIndexType type, const void *data, int memb);

/* define cleanup */
#ifdef GLHCK_WINGDIAPI_DEFINED
#  undef WINGDIAPI
#  undef GLHCK_WINGDIAPI_DEFINED
#endif

#ifdef GLHCK_CALLBACK_DEFINED
#  undef CALLBACK
#  undef GLHCK_CALLBACK_DEFINED
#endif

#ifdef __cplusplus
}
#endif

#endif /* __glhck_h__ */
