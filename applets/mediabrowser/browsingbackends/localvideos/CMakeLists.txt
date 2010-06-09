set(localvideosbackend_SRCS
    localvideosbackend.cpp
)

kde4_add_ui_files(localvideosbackend_SRCS localvideosconfig.ui)
kde4_add_plugin(pmc_localvideos ${localvideosbackend_SRCS})
target_link_libraries(pmc_localvideos mediabrowserlibs ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS})

install(TARGETS pmc_localvideos DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localvideos.desktop DESTINATION ${SERVICES_INSTALL_DIR})