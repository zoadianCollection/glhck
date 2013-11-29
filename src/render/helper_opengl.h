#ifndef __glhck_opengl_helper_h__
#define __glhck_opengl_helper_h__

#include "../internal.h"

#if GLHCK_USE_GLES1
#  define GLEW_USE_LIB_ES11
#elif GLHCK_USE_GLES2
#  define GLEW_USE_LIB_ES20
#endif

#if EMSCRIPTEN
/* FIXME: UGLY HACK, do emscripten support instead */
#  include <SDL_opengl.h>
#  define GL_DEBUG_OUTPUT 0
#  define GLEW_KHR_debug 0
#  define GLEW_VERSION_1_4 0
#  define GLEW_VERSION_2_0 0
#  define GLEW_VERSION_3_0 0
#  define GLEW_EXT_texture_filter_anisotropic 0
#  define GLEW_ES_VERSION_2_0 1
#  define GLEW_OK 0
#  define GLEW_VERSION 0
#  define glewInit() GLEW_OK
#  define glewGetString(x) "EMSCRIPTEN"
#else
#  include "GL/glew.h"
#endif

#ifndef __STRING
#  define __STRING(x) #x
#endif

#ifndef GLchar
#  define GLchar char
#endif

/* check gl errors on debug build */
#ifdef NDEBUG
#  define GL_CALL(x) x
#else
#  define GL_CALL(x) x; GL_ERROR(__LINE__, __func__, __STRING(x));
static void GL_ERROR(unsigned int line, const char *func, const char *glfunc)
{
   GLenum error;
   if ((error = glGetError()) != GL_NO_ERROR)
      DEBUG(GLHCK_DBG_ERROR, "GL @%d:%-20s %-20s >> %s",
            line, func, glfunc,
            error==GL_INVALID_ENUM?
            "GL_INVALID_ENUM":
            error==GL_INVALID_VALUE?
            "GL_INVALID_VALUE":
            error==GL_INVALID_OPERATION?
            "GL_INVALID_OPERATION":
            error==GL_STACK_OVERFLOW?
            "GL_STACK_OVERFLOW":
            error==GL_STACK_UNDERFLOW?
            "GL_STACK_UNDERFLOW":
            error==GL_OUT_OF_MEMORY?
            "GL_OUT_OF_MEMORY":
            error==GL_INVALID_OPERATION?
            "GL_INVALID_OPERATION":
            "GL_UNKNOWN_ERROR");
}
#endif

/* check return value of gl function on debug build */
#ifdef NDEBUG
#  define GL_CHECK(x) x
#else
#  define GL_CHECK(x) GL_CHECK_ERROR(__func__, __STRING(x), x)
static GLenum GL_CHECK_ERROR(const char *func, const char *glfunc, GLenum error)
{
   if (error != GL_NO_ERROR &&
       error != GL_FRAMEBUFFER_COMPLETE)
      DEBUG(GLHCK_DBG_ERROR, "GL @%-20s %-20s >> %s",
            func, glfunc,
            error==GL_FRAMEBUFFER_UNDEFINED?
            "GL_FRAMEBUFFER_UNDEFINED":
            error==GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT?
            "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT":
            error==GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER?
            "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER":
            error==GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER?
            "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER":
            error==GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT?
            "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT":
            error==GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE?
            "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE":
            error==GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS?
            "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS":
            error==GL_FRAMEBUFFER_UNSUPPORTED?
            "GL_FRAMEBUFFER_UNSUPPORTED":
            "GL_UNKNOWN_ERROR");
   return error;
}
#endif

/*** mapping tables ***/
extern GLenum glhckCullFaceTypeToGL[];
extern GLenum glhckFaceOrientationToGL[];
extern GLenum glhckGeometryTypeToGL[];
extern GLenum glhckFramebufferTargetToGL[];
extern GLenum glhckFramebufferAttachmentTypeToGL[];
extern GLenum glhckHwBufferTargetToGL[];
extern GLenum glhckHwBufferStoreTypeToGL[];
extern GLenum glhckHwBufferAccessTypeToGL[];
extern GLenum glhckShaderTypeToGL[];
extern GLenum glhckTextureWrapToGL[];
extern GLenum glhckTextureFilterToGL[];
extern GLenum glhckTextureCompareModeToGL[];
extern GLenum glhckCompareFuncToGL[];
extern GLenum glhckTextureTargetToGL[];
extern GLenum glhckTextureFormatToGL[];
extern GLenum glhckDataTypeToGL[];
extern GLenum glhckBlendingModeToGL[];

/*** check if driver setup is supported ***/
int glhCheckSupport(const char *renderName);

/*** binding mappings ***/
void glhTextureBind(glhckTextureTarget target, GLuint object);
void glhTextureActive(GLuint index);
void glhRenderbufferBind(GLuint object);
void glhFramebufferBind(glhckFramebufferTarget target, GLuint object);
void glhHwBufferBind(glhckHwBufferTarget target, GLuint object);
void glhHwBufferBindBase(glhckHwBufferTarget target, GLuint index, GLuint object);
void glhHwBufferBindRange(glhckHwBufferTarget target, GLuint index, GLuint object, GLintptr offset, GLsizeiptr size);
void glhHwBufferCreate(glhckHwBufferTarget target, GLsizeiptr size, const GLvoid *data, glhckHwBufferStoreType usage);
void glhHwBufferFill(glhckHwBufferTarget target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void* glhHwBufferMap(glhckHwBufferTarget target, glhckHwBufferAccessType access);
void glhHwBufferUnmap(glhckHwBufferTarget target);

/*** shader mapping functions ***/
GLenum glhShaderVariableTypeForGlhckType(_glhckShaderVariableType type);
_glhckShaderVariableType glhGlhckShaderVariableTypeForOpenGLType(GLenum type);
const GLchar* glhShaderVariableNameForOpenGLConstant(GLenum type);
const GLchar* glhShaderVariableNameForGlhckConstant(GLenum type);

/*** shared opengl functions ***/
void glhClear(GLuint bufferBits);
void glhClearColor(const glhckColorb *color);
void glhFrontFace(glhckFaceOrientation orientation);
void glhCullFace(glhckCullFaceType face);
void glhBufferGetPixels(GLint x, GLint y, GLsizei width, GLsizei height, glhckTextureFormat format, glhckDataType type, GLvoid *data);
void glhBlendFunc(GLenum blenda, GLenum blendb);
void glhTextureFill(glhckTextureTarget target, GLint level, GLint x, GLint y, GLint z, GLsizei width, GLsizei height, GLsizei depth, glhckTextureFormat format, GLint datatype, GLsizei size, const GLvoid *data);
void glhTextureImage(glhckTextureTarget target, GLint level, GLsizei width, GLsizei height, GLsizei depth, GLint border, glhckTextureFormat format, GLint datatype, GLsizei size, const GLvoid *data);
void glhTextureParameter(glhckTextureTarget target, const glhckTextureParameters *params);
void glhTextureMipmap(glhckTextureTarget target);
void glhRenderbufferStorage(GLsizei width, GLsizei height, glhckTextureFormat format);
GLint glhFramebufferTexture(glhckFramebufferTarget framebufferTarget, glhckTextureTarget textureTarget, GLuint texture, glhckFramebufferAttachmentType attachment);
GLint glhFramebufferRenderbuffer(glhckFramebufferTarget framebufferTarget, GLuint buffer, glhckFramebufferAttachmentType attachment);
GLuint glhProgramAttachUniformBuffer(GLuint program, const GLchar *uboName, GLuint location);
_glhckHwBufferShaderUniform* glhProgramUniformBufferList(GLuint program, const GLchar *uboName, GLsizei *size);
_glhckShaderAttribute* glhProgramAttributeList(GLuint obj);
_glhckShaderUniform* glhProgramUniformList(GLuint obj);
void glhProgramUniform(GLuint obj, _glhckShaderUniform *uniform, GLsizei count, const GLvoid *value);
void glhGeometryRender(const glhckGeometry *geometry, glhckGeometryType type);

/*** misc ***/
void glhSetupDebugOutput(void);

#endif /* __glhck_opengl_helper_h__ */

/* vim: set ts=8 sw=3 tw=0 :*/
