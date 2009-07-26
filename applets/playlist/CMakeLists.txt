
include_directories(${TAGLIB_INCLUDES} ../../libs ../../)

set(playlist_SRCS
playlistapplet.cpp
playlistwidget.cpp
playlistdelegate.cpp
../../utils/utils.cpp
)
 
# Now make sure all files get to the right place
kde4_add_ui_files(playlist_SRCS config.ui)
kde4_add_plugin(plasma_applet_playlist ${playlist_SRCS})
target_link_libraries(plasma_applet_playlist 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${TAGLIB_LIBRARIES} mediacenterlibs)
 
install(TARGETS plasma_applet_playlist
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-playlist.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
