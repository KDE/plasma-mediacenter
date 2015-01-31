set(favoritesbackend_SRCS
    favoritesbackend.cpp
)

add_library(pmc_favoritesbackend MODULE ${favoritesbackend_SRCS})
set_target_properties(pmc_favoritesbackend PROPERTIES PREFIX "")

target_link_libraries(pmc_favoritesbackend
    plasmamediacenter
)

kcoreaddons_desktop_to_json(pmc_favoritesbackend favoritesbackend.desktop)

# Enable when everything is in order
#install(TARGETS pmc_favoritesbackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
#install(FILES favoritesbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})

