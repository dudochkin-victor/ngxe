FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_mixer REQUIRED)


INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/math/include
    ${PNG_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    )

SET (PROGRAMS l32)

SET(PROGRAM_SRCS
    main.cpp
    )

ADD_EXECUTABLE(${PROGRAMS} ${PROGRAM_SRCS})

   
TARGET_LINK_LIBRARIES(${PROGRAMS}
    ${EXTRA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${SDL_LIBRARY}
    ${SDLMIXER_LIBRARY}
    )

#INSTALL(TARGETS ${PROGRAMS} RUNTIME DESTINATION ${PKG_BINDIR})

