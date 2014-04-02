set(metadatapicturebackend_SRCS
    metadatapicturemodel.cpp
    ../abstractmetadatabackend.cpp
    metadatapicturebackend.cpp
)

add_library(pmc_metadatapicturebackend ${metadatapicturebackend_SRCS})

target_link_libraries(pmc_metadatapicturebackend
    plasmamediacenter
)

install(TARGETS pmc_metadatapicturebackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES metadatapicturebackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
