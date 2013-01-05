FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)

SET(QT_USE_QTDECLARATIVE TRUE)

include (${QT_USE_FILE})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/math/include
    ${CMAKE_SOURCE_DIR}/src/include
    ${PNG_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${QT_INCLUDES}
    )

SET (PROGRAMS lesson03)

SET(PROGRAM_SRCS
    main.cpp
    qmlapplicationviewer.cpp
    Camera.cpp
    noise.cpp
    Matrix3D.cpp
    Quaternion.cpp
    GlslProgram.cpp
    Vector4D.cpp
    Data.cpp
    Frustum.cpp
    Plane.cpp
    Math3D.cpp
    libTexture.cpp
    DdsLoader.cpp
    Texture.cpp
    BmpLoader.cpp
    JpegLoader.cpp
    PngLoader.cpp
    PngLoader.cpp
    TgaLoader.cpp
    CompressedTexture.cpp
    FileSystem.cpp
    ZipFileSystem.cpp
    )
    
SET(PROGRAM_HEADERS
    qmlapplicationviewer.h
    )
        
qt4_wrap_cpp (MOC_SOURCES ${PROGRAM_HEADERS})


ADD_EXECUTABLE(${PROGRAMS} ${PROGRAM_SRCS} ${MOC_SOURCES} ${PROGRAM_HEADERS})

   
TARGET_LINK_LIBRARIES(${PROGRAMS}
    ${EXTRA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARY}
    ${QT_LIBRARIES}
    ${JPEG_LIBRARY}
    -lGLEW
    ngx
    )

INSTALL(TARGETS ${PROGRAMS} RUNTIME DESTINATION ${PKG_BINDIR})

