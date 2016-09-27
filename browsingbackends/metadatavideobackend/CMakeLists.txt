set(metadatavideobackend_SRCS
    metadatavideomodel.cpp
    metadatavideobackend.cpp
)

add_library(pmc_metadatavideobackend MODULE ${metadatavideobackend_SRCS})
set_target_properties(pmc_metadatavideobackend PROPERTIES PREFIX "")

target_link_libraries(pmc_metadatavideobackend
    KMediaCollection
)

kcoreaddons_desktop_to_json(pmc_metadatavideobackend metadatavideobackend.desktop)

install(TARGETS pmc_metadatavideobackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
