set(flickrbackend_SRCS
    flickrmodel.cpp
    flickrbackend.cpp
)

add_library(pmc_flickrbackend MODULE ${flickrbackend_SRCS})
set_target_properties(pmc_flickrbackend PROPERTIES PREFIX "")

target_link_libraries(pmc_flickrbackend
    plasmamediacenter
)

kservice_desktop_to_json(pmc_flickrbackend flickrbackend.desktop)

install(TARGETS pmc_flickrbackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
install(FILES flickrbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
