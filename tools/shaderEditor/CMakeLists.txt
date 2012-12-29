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
	include/AboutDialog.h
	include/App.h
	include/Compulsory.h
	include/globals.h
	include/IdentityTable.h
	include/SGCanvas.h
	include/SGCanvasMouseHandler.h
	include/SGCanvasWrapper.h
	include/SGFixedGLState.h
	include/SGFrame.h
	include/SGModels.h
	include/SGOglFogNBPage.h
	include/SGOglLightNBPage.h
	include/SGOglMaterialNBPage.h
	include/SGOglNotebook.h
	include/SGOglTextureCoordNBPage.h
	include/SGOglTextureEnvNBPage.h
	include/SGShaderGenerator.h
	include/SGShaderTextWindow.h
	include/SGSurfaces.h
	include/SGTextures.h
	include/UtilityFunctions.h
	include/Vector.h
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

