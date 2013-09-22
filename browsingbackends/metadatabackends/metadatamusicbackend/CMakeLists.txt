set(metadatamusicbackend_SRCS
    ../metadatabackendcommonmodel.cpp
    alwaysexpandedmetadatamodel.cpp
    categoriesmodel.cpp
    ../abstractmetadatabackend.cpp
    metadatamusicbackend.cpp
)

qt4_automoc(${metadatamusicbackend_SRCS})

kde4_add_plugin(pmc_metadatamusicbackend ${metadatamusicbackend_SRCS})

target_link_libraries(pmc_metadatamusicbackend
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KFILE_LIBS}
    ${QT_QTDECLARATIVE_LIBRARIES}
    ${NEPOMUK_CORE_LIBRARY}
)

install(TARGETS pmc_metadatamusicbackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES metadatamusicbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})

