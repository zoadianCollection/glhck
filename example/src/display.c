#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef GLHCK_USE_GLES1
#  define GLFW_INCLUDE_ES1
#endif
#ifdef GLHCK_USE_GLES2
#  define GLFW_INCLUDE_ES2
#endif
#include "GL/glfw3.h"
#include "glhck/glhck.h"

static int RUNNING = 0;
static int WIDTH = 800, HEIGHT = 480;
static int MOUSEX = 0, MOUSEY = 0;
static int LASTMOUSEX = 0, LASTMOUSEY = 0;
static int close_callback(GLFWwindow* window)
{
   RUNNING = 0;
   return 1;
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
   WIDTH = width; HEIGHT = height;
   glhckDisplayResize(width, height);
}

static void mousepos_callback(GLFWwindow* window, int mousex, int mousey)
{
   MOUSEX = mousex;
   MOUSEY = mousey;
   if (!LASTMOUSEX && !LASTMOUSEY) {
      LASTMOUSEX = MOUSEX;
      LASTMOUSEY = MOUSEY;
   }
}

static void handleCamera(GLFWwindow* window, float delta, kmVec3 *cameraPos, kmVec3 *cameraRot,
                         glhckProjectionType *projectionType) {
   *projectionType = glfwGetKey(window, GLFW_KEY_P) ?
         GLHCK_PROJECTION_ORTHOGRAPHIC : GLHCK_PROJECTION_PERSPECTIVE;
   if (glfwGetKey(window, GLFW_KEY_W)) {
      cameraPos->x -= cos((cameraRot->y + 90) * kmPIOver180) * 25.0f * delta;
      cameraPos->z += sin((cameraRot->y + 90) * kmPIOver180) * 25.0f * delta;
      cameraPos->y += cos((cameraRot->x + 90) * kmPIOver180) * 25.0f * delta;
   } else if (glfwGetKey(window, GLFW_KEY_S)) {
      cameraPos->x += cos((cameraRot->y + 90) * kmPIOver180) * 25.0f * delta;
      cameraPos->z -= sin((cameraRot->y + 90) * kmPIOver180) * 25.0f * delta;
      cameraPos->y -= cos((cameraRot->x + 90) * kmPIOver180) * 25.0f * delta;
   }

   if (glfwGetKey(window, GLFW_KEY_A)) {
      cameraPos->x -= cos((cameraRot->y + 180) * kmPIOver180) * 25.0f * delta;
      cameraPos->z += sin((cameraRot->y + 180) * kmPIOver180) * 25.0f * delta;
   } else if (glfwGetKey(window, GLFW_KEY_D)) {
      cameraPos->x += cos((cameraRot->y + 180) * kmPIOver180) * 25.0f * delta;
      cameraPos->z -= sin((cameraRot->y + 180) * kmPIOver180) * 25.0f * delta;
   }

   cameraRot->z = 0;
   cameraRot->z -=  glfwGetKey(window, GLFW_KEY_Z) * (float)(MOUSEX - LASTMOUSEX) / 7;
   cameraRot->y -= !glfwGetKey(window, GLFW_KEY_Z) * (float)(MOUSEX - LASTMOUSEX) / 7;
   cameraRot->x += (float)(MOUSEY - LASTMOUSEY) / 7;

   LASTMOUSEX = MOUSEX;
   LASTMOUSEY = MOUSEY;
}

int main(int argc, char **argv)
{
   GLFWwindow* window;
   glhckTexture *texture = NULL;
   glhckObject *cube, *sprite, *cube2, *sprite3, *camObj;
   glhckCamera *camera;
   const kmAABB *aabb;
   kmVec3 cameraPos = { 0, 0, 0 };
   kmVec3 cameraRot = { 0, 0, 0 };
   float spinRadius;
   int queuePrinted = 0;
   float          now          = 0;
   float          last         = 0;
   unsigned int   frameCounter = 0;
   unsigned int   FPS          = 0;
   unsigned int   fpsDelay     = 0;
   float          duration     = 0;
   float          delta        = 0;
   char           WIN_TITLE[256];
   memset(WIN_TITLE, 0, sizeof(WIN_TITLE));

   if (!glfwInit())
      return EXIT_FAILURE;

   puts("-!- glfwinit");

#if GLHCK_USE_GLES1
   glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
   glfwWindowHint(GLFW_DEPTH_BITS, 16);
#elif GLHCK_USE_GLES2
   glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
   glfwWindowHint(GLFW_DEPTH_BITS, 16);
#else
   glfwWindowHint(GLFW_DEPTH_BITS, 24);
#endif
   if (!(window = glfwCreateWindow(WIDTH, HEIGHT, "display test", NULL, NULL)))
      return EXIT_FAILURE;

   glfwMakeContextCurrent(window);
   puts("-!- window create");

   /* Turn on VSYNC if driver allows */
   glfwSwapInterval(0);

   if (!glhckContextCreate(argc, argv))
      return EXIT_FAILURE;

   puts("-!- glhck init");

   if (!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_OPENGL))
      return EXIT_FAILURE;

   puts("-!- glhck display create");

   RUNNING = 1;

   /* test camera */
   if (!(camera = glhckCameraNew()))
      return EXIT_FAILURE;

   glhckCameraRange(camera, 1.0f, 1000.0f);
   camObj = glhckCameraGetObject(camera);

   if (!(texture = glhckTextureNew("example/media/glhck.png", 0, NULL)))
      return EXIT_FAILURE;

   sprite  = glhckSpriteNew(texture, 0, 0);
   sprite3 = glhckSpriteNewFromFile("example/media/glhck.png", 0, 0, 0, NULL);
   cube2   = glhckCubeNew(1.0f);
   glhckObjectMaterialFlags(cube2, glhckObjectGetMaterialFlags(cube2)|
         GLHCK_MATERIAL_DRAW_OBB|GLHCK_MATERIAL_DRAW_AABB);
   glhckObjectScalef(cube2, 1.0f, 1.0f, 2.0f);
   glhckObjectTexture(cube2, texture);

   // FIXME: copy is broken again \o/
   //sprite3 = glhckObjectCopy(sprite);
   glhckObjectScalef(sprite, 0.05f, 0.05f, 0.05f);
   glhckObjectPositionf(sprite3, 64*2, 48*2, 0);

#define SKIP_MMD    1
#define SKIP_OCTM   1
#define SKIP_ASSIMP 0

#if SKIP_MMD
#  define MMD_PATH ""
#else
#  define MMD_PATH "example/media/madoka/md_m.pmd"
#endif

#if SKIP_OCTM
#  define OCTM_PATH ""
#else
#  define OCTM_PATH "example/media/ambulance/ambulance.ctm"
#endif

/* cmake -DGLHCK_IMPORT_ASSIMP=YES */
#if SKIP_ASSIMP
#  define ASSIMP_PATH ""
#else
//#  define ASSIMP_PATH "example/media/chaosgate/chaosgate.obj"
#  define ASSIMP_PATH "example/media/room/room.obj"
#endif

   if ((cube = glhckModelNewEx(MMD_PATH, 1.0f, 0, GLHCK_INDEX_SHORT, GLHCK_VERTEX_V3S))) {
      cameraPos.y =  10.0f;
      cameraPos.z = -40.0f;
   } else if ((cube = glhckModelNewEx(OCTM_PATH, 5.0f, 0, GLHCK_INDEX_SHORT, GLHCK_VERTEX_V3S))) {
      cameraPos.y =  10.0f;
      cameraPos.z = -40.0f;
      glhckObjectPositionf(cube, 0.0f, 5.0f, 0.0f);
   } else if ((cube = glhckModelNewEx(ASSIMP_PATH, 2.0f, 0, GLHCK_INDEX_SHORT, GLHCK_VERTEX_V3S))) {
      cameraPos.y =  10.0f;
      cameraPos.z = -40.0f;
   } else if ((cube = glhckCubeNew(1.0f))) {
      glhckObjectTexture(cube, texture);
      cameraPos.z = -20.0f;
   } else return EXIT_FAILURE;

#if 0
   glhckShader *shader = glhckShaderNew(NULL, "VSM.GLhck.Lighting.ShadowMapping.Unpacking.Fragment", NULL);
   glhckShader *depthShader = glhckShaderNew(NULL, "VSM.GLhck.Depth.Packing.Fragment", NULL);
   glhckShader *depthRenderShader = glhckShaderNew(NULL, "VSM.GLhck.DepthRender.Unpacking.Fragment", NULL);
   glhckShaderSetUniform(shader, "ShadowMap", 1, &((int[]){1}));

   int sW = 1024, sH = 1024;
   glhckRenderbuffer *depthBuffer = glhckRenderbufferNew(sW, sH, GLHCK_DEPTH_COMPONENT16);
   glhckTexture *depthColorMap = glhckTextureNew(NULL, 0);
   glhckTextureCreate(depthColorMap, GLHCK_TEXTURE_2D, NULL, sW, sH, 0, GLHCK_RGBA, GLHCK_RGBA, 0);
   glhckFramebuffer *fbo = glhckFramebufferNew(GLHCK_FRAMEBUFFER);
   glhckFramebufferAttachTexture(fbo, depthColorMap, GLHCK_COLOR_ATTACHMENT0);
   glhckFramebufferAttachRenderbuffer(fbo, depthBuffer, GLHCK_DEPTH_ATTACHMENT);
   glhckFramebufferRecti(fbo, 0, 0, sW, sH);

   glhckObject *screen = glhckSpriteNew(depthColorMap, 128, 128);
   glhckObjectShader(screen, depthRenderShader);
   glhckObjectMaterialFlags(screen, 0);
#endif

   glhckObject *plane = glhckPlaneNew(200.0f, 200.0f);
   glhckObjectTexture(plane, texture);
   glhckObjectPositionf(plane, 0.0f, -0.1f, 0.0f);
   glhckObjectRotationf(plane, 90.0f, 0.0f, 0.0f);

   unsigned int flags = glhckObjectGetMaterialFlags(cube);
   flags |= GLHCK_MATERIAL_DRAW_AABB;
   flags |= GLHCK_MATERIAL_DRAW_OBB;
   //glhckObjectMaterialFlags(cube, flags);

   glhckText *text = glhckTextNew(512, 512);
   if (!text) return EXIT_FAILURE;

   unsigned int font  = glhckTextNewFont(text, "example/media/sazanami-gothic.ttf");
   unsigned int font2 = glhckTextNewFont(text, "example/media/DejaVuSans.ttf");

   glhckObject *rttText = NULL, *rttText2 = NULL;
   glhckTextColor(text, 255, 255, 255, 255);
   if ((rttText = glhckTextPlane(text, font2, 42, "RTT Text", NULL))) {
      glhckObjectScalef(rttText, 0.05f, 0.05f, 1.0f); /* scale to fit our 3d world */
      glhckObjectPositionf(rttText, 0, 2, 0);
      glhckObjectAddChildren(cube2, rttText);

      /* ignore lighting */
      glhckObjectMaterialFlags(rttText, GLHCK_MATERIAL_CULL | GLHCK_MATERIAL_DEPTH);
   }

   if ((rttText2 = glhckTextPlane(text, font, 42, "GLHCKあいしてる", NULL))) {
      glhckObjectRotatef(rttText2, 0, 210, 0);
      glhckObjectPositionf(rttText2, 200, 0, 500);

      /* ignore lighting */
      glhckObjectMaterialFlags(rttText2, GLHCK_MATERIAL_CULL | GLHCK_MATERIAL_DEPTH);
   }

   glfwSetWindowCloseCallback(window, close_callback);
   glfwSetWindowSizeCallback(window, resize_callback);
   glfwSetCursorPosCallback(window, mousepos_callback);
   glfwSetInputMode(window, GLFW_CURSOR_MODE, GLFW_CURSOR_CAPTURED);

   glhckProjectionType projectionType = GLHCK_PROJECTION_PERSPECTIVE;

   int numLights = 4, li;
   glhckLight *light[numLights];
   for (li = 0; li != numLights; ++li) {
      light[li] = glhckLightNew();
      glhckLightCutoutf(light[li], 45.0f, 0.0f);
      glhckLightPointLightFactor(light[li], 0.1f);
      glhckLightColorb(light[li], rand()%255, rand()%255, rand()%255, 255);
      glhckObject *c = glhckCubeNew(1.0f);
      glhckObjectAddChildren(glhckLightGetObject(light[li]), c);
      glhckObjectTexture(c, texture);
      glhckObjectFree(c);
   }

   glhckMemoryGraph();

   float xspin = 0.0f;
   while (RUNNING && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
      last  =  now;
      now   =  glfwGetTime();
      delta =  now - last;
      glhckRenderTime(now);

      glfwPollEvents();
      handleCamera(window, delta, &cameraPos, &cameraRot, &projectionType);
      glhckCameraProjection(camera, projectionType);

      /* update the camera */
      if (glfwGetKey(window, GLFW_KEY_O)) {
         kmMat4 identity;
         kmMat4Identity(&identity);
         glhckRenderProjection(&identity);
      } else if (glfwGetKey(window, GLFW_KEY_I)) {
         kmMat4 mat2d, pos;
         glhckCameraUpdate(camera);
         kmMat4Translation(&pos, -cameraPos.x, -cameraPos.y, -cameraPos.z);
         kmMat4Scaling(&mat2d, -2.0f/WIDTH, 2.0f/HEIGHT, 0.0f);
         kmMat4Multiply(&mat2d, &mat2d, &pos);
         glhckRenderProjection(&mat2d);
      } else {
         glhckCameraUpdate(camera);
      }

      /* rotate */
      glhckObjectPosition(camObj, &cameraPos);
      glhckObjectTargetf(camObj, cameraPos.x, cameraPos.y, cameraPos.z + 1);
      glhckObjectRotate(camObj, &cameraRot);

      /* target */
      //glhckObjectTarget(cube, glhckObjectGetPosition(sprite));

      /* do spinning effect */
      aabb = glhckObjectGetAABB(cube);
      spinRadius = aabb->max.x>aabb->max.z?
         aabb->max.x>aabb->max.y?aabb->max.x:aabb->max.y:
         aabb->max.z>aabb->max.y?aabb->max.z:aabb->max.y;
      spinRadius += 5.0f;

      if (glfwGetKey(window, GLFW_KEY_SPACE)) xspin += 30.0f * delta;
      glhckObjectPositionf(sprite,
            spinRadius*sin(xspin/8),
            spinRadius*cos(xspin/12),
            spinRadius);

      glhckObjectPositionf(cube2,
            spinRadius*sin(xspin/20),
            spinRadius*cos(xspin/20),
            spinRadius*sin(xspin/40));
      glhckObjectTarget(cube2, glhckObjectGetPosition(cube));
      glhckObjectTarget(sprite3, glhckObjectGetPosition(camObj));

      for (li = 0; li != numLights; ++li) {
#if 1
         glhckObjectPositionf(glhckLightGetObject(light[li]),
               sin(xspin*(li+1)*0.01)*85.0f, sin(xspin*0.01)*30.0f+60.0f, cos(xspin*(li+1)*0.01)*85.0f);
#else
         glhckObjectPositionf(glhckLightGetObject(light[li]),
               sin(li+1)*80.0f, sin(xspin*0.1)*30.0f+45.0f, cos(li+1)*80.0f);
#endif
         glhckObjectTargetf(glhckLightGetObject(light[li]), 0, 0, 0);

#if 0
         glhckRenderPassShader(NULL);
         glhckRenderRenderBlendFunc(GLHCK_ZERO, GLHCK_ZERO);
         glhckRenderPassFlags(GLHCK_PASS_DEPTH | GLHCK_PASS_CULL);
         glhckRenderPassShader(depthShader);

         glhckObjectDraw(cube);
         glhckObjectDraw(plane);

         glhckCameraRange(camera, 5.0f, 250.0f);
         glhckLightBeginProjectionWithCamera(light[li], camera);
         glhckLightBind(light[li]);
         glhckFramebufferBegin(fbo);
         glhckClear(GLHCK_DEPTH_BUFFER | GLHCK_COLOR_BUFFER);
         glhckRender();
         glhckFramebufferEnd(fbo);
         glhckLightEndProjectionWithCamera(light[li], camera);

         glActiveTexture(GL_TEXTURE1);
         glhckTextureBind(depthColorMap);
         glActiveTexture(GL_TEXTURE0);

         glhckCameraRange(camera, 1.0f, 1000.0f);
         glhckCameraUpdate(camera);

         glhckRenderPassFlags(GLHCK_PASS_DEFAULTS);
         glhckRenderPassShader(shader);
#else
         glhckLightBeginProjectionWithCamera(light[li], camera);
         glhckLightBind(light[li]);
         glhckLightEndProjectionWithCamera(light[li], camera);
#endif

         glhckObjectDraw(rttText2);
         glhckObjectDraw(sprite);
         glhckObjectDraw(sprite3);
         glhckObjectDraw(cube2);
         glhckObjectDraw(cube);
         glhckObjectDraw(plane);
         glhckObjectDraw(glhckLightGetObject(light[li]));

         if (li) glhckRenderBlendFunc(GLHCK_ONE, GLHCK_ONE);
         glhckRender();
      }
      glhckRenderBlendFunc(GLHCK_ZERO, GLHCK_ZERO);

      /* debug */
      if (!queuePrinted) {
         glhckRenderPrintTextureQueue();
         glhckRenderPrintObjectQueue();
         queuePrinted = 1;
      }

      /* render frustum */
      glhckFrustumRender(glhckCameraGetFrustum(camera));

#if 0
      kmMat4 mat2d;
      kmMat4Scaling(&mat2d, -2.0f/WIDTH, 2.0f/HEIGHT, 0.0f);
      glhckRenderProjection(&mat2d);
      glhckObjectPositionf(screen, WIDTH/2.0f-128.0f/2.0f, HEIGHT/2.0f-128.0f/2.0f, 0);
      glhckObjectRender(screen);
#endif

      /* draw some text */
      glhckTextColor(text, 255, 255, 255, 255);
      glhckTextDraw(text, font2, 18,         0,  HEIGHT-4, WIN_TITLE, NULL);
      glhckTextDraw(text, font,  42,        25, HEIGHT-80, "愛してるGLHCK", NULL);
      glhckTextDraw(text, font2, 32,        25, HEIGHT-40, "Äöäö DejaVuSans perkele", NULL);
      glhckTextDraw(text, font2, 18,         0,        18, "SADASD!?,.:;", NULL);
      glhckTextRender(text);

      glhckTextColor(text, 255, 0, 0, 255);
      glhckTextDraw(text, font2, 12, WIDTH-100,        18, "Wall of text", NULL);
      glhckTextRender(text);

      /* actual swap and clear */
      glfwSwapBuffers(window);
      glhckRenderClear(GLHCK_COLOR_BUFFER | GLHCK_DEPTH_BUFFER);

      /* fps calc */
      if (fpsDelay < now) {
         if (duration > 0.0f) {
            FPS = (float)frameCounter / duration;
            snprintf(WIN_TITLE, sizeof(WIN_TITLE)-1, "OpenGL [FPS: %d]", FPS);
            glfwSetWindowTitle(window, WIN_TITLE);
            frameCounter = 0; fpsDelay = now + 1; duration = 0;
         }
      }

      ++frameCounter;
      duration += delta;
   }

   /* check that everything is freed
    * like should */
   glhckObjectFree(cube);
   glhckObjectFree(sprite);
   glhckObjectFree(cube2);
   glhckObjectFree(sprite3);
   if (rttText)  glhckObjectFree(rttText);
   if (rttText2) glhckObjectFree(rttText2);
   if (texture)  glhckTextureFree(texture);
   glhckCameraFree(camera);
   glhckTextFree(text);

   puts("GLHCK channel should have few bytes for queue list,\n"
        "which is freed on glhckTerminate");
   glhckMemoryGraph();

   /* should cleanup all
    * objects as well */
   glhckContextTerminate();
   glfwTerminate();
   return EXIT_SUCCESS;
}

/* vim: set ts=8 sw=3 tw=0 :*/
