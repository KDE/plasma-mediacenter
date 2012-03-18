
set(playlist_engine_SRCS
playlist.cpp
playlistservice.cpp
playlistjob.cpp
)

qt4_add_dbus_adaptor(playlist_engine_SRCS org.kde.PlaylistEngine.xml playlist.h PlaylistEngine)

kde4_add_plugin(plasma_engine_playlist ${playlist_engine_SRCS})
target_link_libraries(plasma_engine_playlist ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_engine_playlist DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-playlist.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES playlist.operations DESTINATION ${DATA_INSTALL_DIR}/plasma/services)
