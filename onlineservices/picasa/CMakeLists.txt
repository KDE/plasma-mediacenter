set(picasabackend_SRCS
picasabackend.cpp
picasamodel.cpp
)

add_library(pmc_picasabackend MODULE ${picasabackend_SRCS})
set_target_properties(pmc_picasabackend PROPERTIES PREFIX "")

target_link_libraries(pmc_picasabackend
    plasmamediacenter
)

kcoreaddons_desktop_to_json(pmc_picasabackend picasabackend.desktop)

install(TARGETS pmc_picasabackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/searchbackends)
install(DIRECTORY picasacomponents DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/mediacenter/elements)
