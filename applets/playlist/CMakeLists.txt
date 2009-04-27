set(playlist_SRCS
playlist.cpp
playlistwidget.cpp
)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_playlist ${playlist_SRCS})
target_link_libraries(plasma_applet_playlist 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
 
install(TARGETS plasma_applet_playlist
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-playlist.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
