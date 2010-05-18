set(localvideospackage_SRCS
    localvideospackage.cpp
)

kde4_add_ui_files(localvideospackage_SRCS localvideosconfig.ui)
kde4_add_plugin(mc_localvideos ${localvideospackage_SRCS})
target_link_libraries(mc_localvideos mediabrowserlibs ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS})

install(TARGETS mc_localvideos DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localvideos.desktop DESTINATION ${SERVICES_INSTALL_DIR})