FIND_PACKAGE(wxWidgets REQUIRED net gl core base)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE(${wxWidgets_USE_FILE})

SET (wxWidgets_USE_DEBUG)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/math/include
    ${wxWidgets_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIR}
    )

SET (PROGRAMS shaderEditor)

SET(SHED_SRCS
    AboutDialog.cpp
    App.cpp
    SGCanvas.cpp
    SGCanvasMouseHandler.cpp
    SGCanvasWrapper.cpp
    SGFixedGLState.cpp
    SGFrame.cpp
    SGModels.cpp
    SGOglFogNBPage.cpp
    SGOglLightNBPage.cpp
    SGOglMaterialNBPage.cpp
    SGOglNotebook.cpp
    SGOglTextureCoordNBPage.cpp
    SGOglTextureEnvNBPage.cpp
    SGShaderGenerator.cpp
    SGShaderTextWindow.cpp
    SGSurfaces.cpp
    SGTextures.cpp
    UtilityFunctions.cpp
    Vector.cpp
    )

SET(SHED_HEADERS
	AboutDialog.h
	App.h
	Compulsory.h
	globals.h
	IdentityTable.h
	SGCanvas.h
	SGCanvasMouseHandler.h
	SGCanvasWrapper.h
	SGFixedGLState.h
	SGFrame.h
	SGModels.h
	SGOglFogNBPage.h
	SGOglLightNBPage.h
	SGOglMaterialNBPage.h
	SGOglNotebook.h
	SGOglTextureCoordNBPage.h
	SGOglTextureEnvNBPage.h
	SGShaderGenerator.h
	SGShaderTextWindow.h
	SGSurfaces.h
	SGTextures.h
	UtilityFunctions.h
	Vector.h
    )
ADD_EXECUTABLE(shaderEditor ${SHED_SRCS} ${SHED_HEADERS})

   
TARGET_LINK_LIBRARIES(shaderEditor
    ${wxWidgets_LIBRARIES}
    ${PNG_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLU_LIBRARY}
    -lGLEW
    )

INSTALL(TARGETS shaderEditor RUNTIME DESTINATION ${PKG_BINDIR})

