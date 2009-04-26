
set(mediaplayer_engine_SRCS mediaplayer.cpp)
# qt4_add_dbus_adaptor(mediaplayer_engine_SRCS org.kde.PlaylistEngine.xml mediaplayer.h PlaylistEngine)

kde4_add_plugin(plasma_engine_mediaplayer ${mediaplayer_engine_SRCS})
target_link_libraries(plasma_engine_mediaplayer ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_PHONON_LIBS})

install(TARGETS plasma_engine_mediaplayer DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-mediaplayer.desktop DESTINATION ${SERVICES_INSTALL_DIR})