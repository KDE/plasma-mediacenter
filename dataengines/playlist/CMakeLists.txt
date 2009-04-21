project(playlist-engine)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES}
)

set(playlist_engine_SRCS playlist.cpp)
qt4_add_dbus_adaptor(playlist_engine_SRCS org.kde.PlaylistEngine.xml playlist.h PlaylistEngine)

kde4_add_plugin(plasma_engine_playlist ${playlist_engine_SRCS})
target_link_libraries(plasma_engine_playlist ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_engine_playlist DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-playlist.desktop DESTINATION ${SERVICES_INSTALL_DIR})
