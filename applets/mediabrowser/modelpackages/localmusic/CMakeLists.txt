set(localmusicpackage_SRCS
    localmusicpackage.cpp
)

kde4_add_ui_files(localmusicpackage_SRCS localmusicconfig.ui)
kde4_add_plugin(mc_localmusic ${localmusicpackage_SRCS})
target_link_libraries(mc_localmusic mediabrowserlibs ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS})

install(TARGETS mc_localmusic DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localmusic.desktop DESTINATION ${SERVICES_INSTALL_DIR})