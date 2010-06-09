set(localmusicbackend_SRCS
    localmusicbackend.cpp
)

kde4_add_ui_files(localmusicbackend_SRCS localmusicconfig.ui)
kde4_add_plugin(pmc_localmusic ${localmusicbackend_SRCS})
target_link_libraries(pmc_localmusic mediabrowserlibs ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS})

install(TARGETS pmc_localmusic DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localmusic.desktop DESTINATION ${SERVICES_INSTALL_DIR})