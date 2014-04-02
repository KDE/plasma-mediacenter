set(metadatamusicbackend_SRCS
    metadatamusicsongsmodel.cpp
    ../metadatabackendcommonmodel.cpp
    alwaysexpandedmetadatamodel.cpp
    categoriesmodel.cpp
    ../abstractmetadatabackend.cpp
    metadatamusicbackend.cpp
)

add_library(pmc_metadatamusicbackend ${metadatamusicbackend_SRCS})

target_link_libraries(pmc_metadatamusicbackend
    plasmamediacenter
)

install(TARGETS pmc_metadatamusicbackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES metadatamusicbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})

