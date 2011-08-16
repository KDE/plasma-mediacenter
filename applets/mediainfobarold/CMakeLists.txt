
set(mediainfobar_SRCS
mediainfobar.cpp
)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_mediainfobar ${mediainfobar_SRCS})
target_link_libraries(plasma_applet_mediainfobar
                      ${KDE4_PLASMA_LIBS}
                      mediacenterlibs
                      ${KDE4_PHONON_LIBS}
                      ${KDE4_NEPOMUK_LIBS}
)

install(TARGETS plasma_applet_mediainfobar
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-mediainfobar.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
