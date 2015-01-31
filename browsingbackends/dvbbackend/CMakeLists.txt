set(dvbbackend_SRCS
    dvbbackend.cpp
)

add_library(pmc_dvbbackend MODULE ${dvbbackend_SRCS})
set_target_properties(pmc_dvbbackend PROPERTIES PREFIX "")

target_link_libraries(pmc_dvbbackend
    plasmamediacenter
)

kcoreaddons_desktop_to_json(pmc_dvbbackend dvbbackend.desktop)

# Enable when everything is in order
#install(TARGETS pmc_dvbbackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
#install(FILES dvbbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})

