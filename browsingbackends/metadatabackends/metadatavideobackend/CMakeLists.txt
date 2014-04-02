set(metadatavideobackend_SRCS
    ../metadatabackendcommonmodel.cpp
    metadatavideomodel.cpp
    ../abstractmetadatabackend.cpp
    metadatavideobackend.cpp
)

add_library(pmc_metadatavideobackend ${metadatavideobackend_SRCS})

target_link_libraries(pmc_metadatavideobackend
    plasmamediacenter
)

install(TARGETS pmc_metadatavideobackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES metadatavideobackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
