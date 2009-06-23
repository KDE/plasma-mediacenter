
set(containment_SRCS
mediacontainment.cpp
medianotificationwidget.cpp
medialayout.cpp
)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_mediacontainment ${containment_SRCS})
target_link_libraries(plasma_applet_mediacontainment
                      ${KDE4_PLASMA_LIBS}
                      mediacenterlibs
)
 
install(TARGETS plasma_applet_mediacontainment
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-mediacontainment.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
