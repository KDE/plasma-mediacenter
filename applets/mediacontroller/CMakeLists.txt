
set(controller_SRCS
controller.cpp
)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_mediacontroller ${controller_SRCS})
target_link_libraries(plasma_applet_mediacontroller
                      ${KDE4_PLASMA_LIBS}
                      mediacenterlibs
                      ${KDE4_PHONON_LIBS}
)
 
install(TARGETS plasma_applet_mediacontroller
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-mediacontroller.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
