SET(EXAMPLE_EXE
   display
   distanceField
   game)

FOREACH (example ${EXAMPLE_EXE})
   PROJECT(${example})
   ADD_EXECUTABLE(${example} src/${example}.c)
   INCLUDE_DIRECTORIES(
      ${glhck_SOURCE_DIR}/include
      ${GLFW_SOURCE_DIR}/include
      ${kazmath_SOURCE_DIR}/src
   )
   TARGET_LINK_LIBRARIES(${example} glfw glhck ${GLHCK_LIBRARIES} ${GLFW_LIBRARIES} ${EGL_LIBRARY})
   INSTALL(TARGETS ${example} DESTINATION example)
ENDFOREACH (example)

INSTALL(DIRECTORY media DESTINATION example)
