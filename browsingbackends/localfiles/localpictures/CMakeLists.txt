set(localpicturesbackend_SRCS
    localpicturesmodel.cpp
    ../localfilesabstractbackend.cpp
    ../localfilesabstractmodel.cpp
    ../localplacesmodel.cpp
    localpicturesbackend.cpp
     ../localvideos/localthumbnailprovider.cpp
)

kde4_add_plugin(pmc_localpictures ${localpicturesbackend_SRCS})
target_link_libraries(pmc_localpictures
plasmamediacenter
${KDE4_KDECORE_LIBS}
${KDE4_KIO_LIBS}
${KDE4_KFILE_LIBS}
${QT_QTDECLARATIVE_LIBRARIES}
${KDE4_SOLID_LIBS}
)

install(TARGETS pmc_localpictures DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localpictures.desktop DESTINATION ${SERVICES_INSTALL_DIR})
