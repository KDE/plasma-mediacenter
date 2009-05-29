
set(mediabrowser_SRCS
mediabrowser.cpp
abstractmediaitemview.cpp
listview.cpp
)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_mediabrowser ${mediabrowser_SRCS})
target_link_libraries(plasma_applet_mediabrowser
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KIO_LIBS}
                        )
 
install(TARGETS plasma_applet_mediabrowser
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-mediabrowser.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
