set(fakemediasource_SRCS
    fakemediasource.cpp
)

add_library(pmc_fakemediasource MODULE ${fakemediasource_SRCS})
set_target_properties(pmc_fakemediasource PROPERTIES PREFIX "")

target_link_libraries(pmc_fakemediasource
    plasmamediacenter
)

kcoreaddons_desktop_to_json(pmc_fakemediasource fakemediasource.desktop)

install(TARGETS pmc_fakemediasource DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/mediasources)
install(FILES medialist.json DESTINATION ${DATA_INSTALL_DIR}/plasma/mediacenter/mediasources)
