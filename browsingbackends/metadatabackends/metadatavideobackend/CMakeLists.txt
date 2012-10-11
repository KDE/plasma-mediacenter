set(metadatavideobackend_SRCS
    metadatavideomodel.cpp
    ../abstractmetadatabackend.cpp
    metadatavideobackend.cpp
)

qt4_automoc(${metadatavideobackend_SRCS})

kde4_add_plugin(pmc_metadatavideobackend ${metadatavideobackend_SRCS})

target_link_libraries(pmc_metadatavideobackend
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARIES}
)

install(TARGETS pmc_metadatavideobackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES metadatavideobackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
