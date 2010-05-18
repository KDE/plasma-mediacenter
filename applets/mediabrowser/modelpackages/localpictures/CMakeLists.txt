set(localpicturespackage_SRCS
    localpicturespackage.cpp
)

kde4_add_ui_files(localpicturespackage_SRCS localpicturesconfig.ui)
kde4_add_plugin(mc_localpictures ${localpicturespackage_SRCS})
target_link_libraries(mc_localpictures mediabrowserlibs ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS})

install(TARGETS mc_localpictures DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localpictures.desktop DESTINATION ${SERVICES_INSTALL_DIR})