#FIND_PACKAGE(MySQL REQUIRED)
#FIND_PACKAGE(CRYPTOPP REQUIRED)
#FIND_PACKAGE(MYSQLPP REQUIRED)
#FIND_PACKAGE(LibXml2 REQUIRED)
#FIND_PACKAGE(LibEvent REQUIRED)
#FIND_PACKAGE( Boost 1.36.0 COMPONENTS regex )
FIND_PACKAGE(FREEALUT REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(Freetype REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)

#ADD_SUBDIRECTORY(service)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/game/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/math/include
#    ${MYSQL_INCLUDE_DIR}
#    ${MYSQLPP_INCLUDE_DIR}
#    ${LIBXML2_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    )

SET (LIBRARY ngx)

SET(LIBRARY_SRCS
    Engine.cpp
    View.cpp
    Video.cpp
    Sound.cpp
    Input.cpp
    Util.cpp
    IMAGE.cpp
    TIMER.cpp
    FPS_COUNTER.cpp
    FontManager.cpp
    IMAGE.cpp
    FreeType.cpp
    Maths/COLOR.cpp
    Maths/PLANE.cpp
    Maths/MATRIX4X4.cpp
    Maths/VECTOR2D.cpp
    Maths/VECTOR3D.cpp
    Maths/VECTOR4D.cpp
    extensions/ARB_multitexture_extension.cpp
    extensions/ARB_texture_border_clamp_extension.cpp
    extensions/ARB_texture_cube_map_extension.cpp
    extensions/ARB_texture_env_combine_extension.cpp
    extensions/ARB_texture_env_dot3_extension.cpp
    extensions/ARB_vertex_program_extension.cpp
    extensions/EXT_blend_minmax_extension.cpp
    extensions/EXT_blend_subtract_extension.cpp
    extensions/EXT_compiled_vertex_array_extension.cpp
    extensions/EXT_stencil_two_side_extension.cpp
    extensions/EXT_stencil_wrap_extension.cpp
    extensions/EXT_texture3D_extension.cpp
    extensions/EXT_texture_edge_clamp_extension.cpp
    extensions/EXT_texture_env_combine_extension.cpp
    extensions/EXT_texture_filter_anisotropic_extension.cpp
    extensions/NV_register_combiners2_extension.cpp
    extensions/NV_register_combiners_extension.cpp
    extensions/NV_texture_shader_extension.cpp
    extensions/NV_vertex_program_extension.cpp
    extensions/SGIS_generate_mipmap_extension.cpp
    extensions/SGIX_depth_texture_extension.cpp
    extensions/SGIX_shadow_extension.cpp
    extensions/WGL_ARB_extensions_string_extension.cpp
    extensions/WGL_ARB_pbuffer_extension.cpp
    extensions/WGL_ARB_pixel_format_extension.cpp
    extensions/WGL_ARB_render_texture_extension.cpp
    )

#set (INTERNAL_LIBRARIES ${INTERNAL_LIBRARIES}
#    crgame
#    crengine
#    crmath
#    )

SET(LIBRARY_HEADERS
    include/Engine.h
    )
ADD_LIBRARY(${LIBRARY} SHARED ${LIBRARY_SRCS} ${LIBRARY_HEADERS})

TARGET_LINK_LIBRARIES(${LIBRARY}
    ${INTERNAL_LIBRARIES}
#    ${MYSQL_LIBRARIES}
#    ${MYSQLPP_LIBRARIES}
#    ${CRYPTOPP_LIBRARIES}
#    ${LIBEVENT_LIBRARIES}
#    ${LIBXML2_LIBRARIES}
#    ${LIBINTL_LIBRARIES}
#    ${Boost_LIBRARIES}
    ${OPENAL_LIBRARIES}
    ${FREEALUT_LIBRARY}
    ${GLEW_LIBRARY}
    ${FREETYPE_LIBRARY}
    ${JPEG_LIBRARY}
    ${PNG_LIBRARY}
    ${EXTRA_LIBRARIES})

#INSTALL(TARGETS jazzd RUNTIME DESTINATION ${PKG_BINDIR})
#INSTALL(FILES ${JAZZ_HEADERS} DESTINATION include)

