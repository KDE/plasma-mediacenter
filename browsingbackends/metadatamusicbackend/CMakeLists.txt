set(metadatamusicbackend_SRCS
    metadatamusicbackend.cpp
)

add_library(pmc_metadatamusicbackend MODULE ${metadatamusicbackend_SRCS})
set_target_properties(pmc_metadatamusicbackend PROPERTIES PREFIX "")

target_link_libraries(pmc_metadatamusicbackend
    libmedia
)

kcoreaddons_desktop_to_json(pmc_metadatamusicbackend metadatamusicbackend.desktop)

install(TARGETS pmc_metadatamusicbackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
