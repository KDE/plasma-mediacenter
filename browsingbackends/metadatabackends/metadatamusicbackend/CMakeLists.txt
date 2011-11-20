set(metadatamusicbackend_SRCS
    metadatamusicmodel.cpp
    ../abstractmetadatabackend.cpp
    ../abstractmetadatamodel.cpp
    metadatamusicbackend.cpp
)

qt4_automoc(${metadatamusicbackend_SRCS})

kde4_add_plugin(pmc_metadatamusicbackend ${metadatamusicbackend_SRCS})

target_link_libraries(pmc_metadatamusicbackend
    mediacenterlibs
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KFILE_LIBS}
)

install(TARGETS pmc_metadatamusicbackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES metadatamusicbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
