# We add our source code here
set(coverfetcher_SRCS
coverfetcher.cpp
lastfmfetcher.cpp
coverjob.cpp
coverservice.cpp
)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_engine_coverfetcher ${coverfetcher_SRCS})
target_link_libraries(plasma_engine_coverfetcher
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KIO_LIBS}
)
 
install(TARGETS plasma_engine_coverfetcher
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-engine-coverfetcher.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES coverfetcher.operations DESTINATION ${DATA_INSTALL_DIR}/plasma/services)