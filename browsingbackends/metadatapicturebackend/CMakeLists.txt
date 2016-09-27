set(metadatapicturebackend_SRCS
    metadatapicturemodel.cpp
    metadatapicturebackend.cpp
)

add_library(pmc_metadatapicturebackend MODULE ${metadatapicturebackend_SRCS})
set_target_properties(pmc_metadatapicturebackend PROPERTIES PREFIX "")

target_link_libraries(pmc_metadatapicturebackend
    libmedia
)

kcoreaddons_desktop_to_json(pmc_metadatapicturebackend metadatapicturebackend.desktop)

install(TARGETS pmc_metadatapicturebackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
