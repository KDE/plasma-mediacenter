set(metadatavideobackend_SRCS
    metadatavideothumbnailprovider.cpp
    metadatavideomodel.cpp
    ../abstractmetadatabackend.cpp
    ../abstractmetadatamodel.cpp
    metadatavideobackend.cpp
)

qt4_automoc(${metadatavideobackend_SRCS})

kde4_add_plugin(pmc_metadatavideobackend ${metadatavideobackend_SRCS})

target_link_libraries(pmc_metadatavideobackend
    mediacenterlibs
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KFILE_LIBS}
    ${QT_QTDECLARATIVE_LIBRARIES}
)

install(TARGETS pmc_metadatavideobackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES metadatavideobackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
