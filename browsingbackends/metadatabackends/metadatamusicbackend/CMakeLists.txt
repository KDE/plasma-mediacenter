set(metadatamusicbackend_SRCS
    metadatamusicsongsmodel.cpp
    ../metadatabackendcommonmodel.cpp
    alwaysexpandedmetadatamodel.cpp
    categoriesmodel.cpp
    ../abstractmetadatabackend.cpp
    metadatamusicbackend.cpp
)

add_library(pmc_metadatamusicbackend MODULE ${metadatamusicbackend_SRCS})
set_target_properties(pmc_metadatamusicbackend PROPERTIES PREFIX "")

target_link_libraries(pmc_metadatamusicbackend
    plasmamediacenter
)

kservice_desktop_to_json(pmc_metadatamusicbackend metadatamusicbackend.desktop)

install(TARGETS pmc_metadatamusicbackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
install(FILES metadatamusicbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})

