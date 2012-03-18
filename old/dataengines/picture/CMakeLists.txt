include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
)

set(picture_engine_SRCS
picture.cpp
)

kde4_add_plugin(plasma_engine_picture ${picture_engine_SRCS})
target_link_libraries(plasma_engine_picture pictureprovidercore ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS plasma_engine_picture DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-picture.desktop DESTINATION ${SERVICES_INSTALL_DIR})

set(pictureprovider_core_SRCS
    pictureprovider/pictureprovider.cpp
)

kde4_add_library(pictureprovidercore SHARED ${pictureprovider_core_SRCS})
target_link_libraries(pictureprovidercore ${KDE4_KDECORE_LIBS})
install(TARGETS pictureprovidercore ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES pictureprovider.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

set (pictureprovider_core_HDRS
    pictureprovider/pictureprovider.h
)

install(FILES ${pictureprovider_core_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/pictureprovider COMPONENT Devel)
