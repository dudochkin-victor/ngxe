FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)


INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/include
    ${JPEG_INCLUDE_DIR}
    )

SET (CMAKE_BUILD_TYPE Debug)
SET (PROGRAMS q3bsp)

SET(PROGRAM_SRCS
    BITSET.cpp
    BSP.cpp
    FPS_COUNTER.cpp
    FRUSTUM.cpp
    IMAGE.cpp
    jpeg.cpp
    LOG.cpp
    main.cpp
    TIMER.cpp
    WALKING_CAMERA.cpp
    extensions/EXT_draw_range_elements_extension.cpp
    extensions/EXT_multi_draw_arrays_extension.cpp
    extensions/EXT_texture_env_combine_extension.cpp
    Maths/PLANE.cpp
    Maths/MATRIX4X4.cpp
    Maths/VECTOR3D.cpp
    )

ADD_EXECUTABLE(${PROGRAMS} ${PROGRAM_SRCS})

   
TARGET_LINK_LIBRARIES(${PROGRAMS}
    ngx
    ${EXTRA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${GLEW_LIBRARY}
    ${JPEG_LIBRARY}
    )

#INSTALL(TARGETS ${PROGRAMS} RUNTIME DESTINATION ${PKG_BINDIR})


