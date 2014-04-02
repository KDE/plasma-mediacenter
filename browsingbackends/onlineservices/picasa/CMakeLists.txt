set(picasabackend_SRCS
picasabackend.cpp
picasamodel.cpp
)

add_library(pmc_picasabackend ${picasabackend_SRCS})

target_link_libraries(pmc_picasabackend
    plasmamediacenter
)

install(TARGETS pmc_picasabackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES picasabackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(DIRECTORY picasacomponents DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/mediacenter/elements)
