set(youtubebackend_SRCS
    diskcache.cpp
    networkaccess.cpp
    videodefinition.cpp
    networkaccess.cpp
    video.cpp
    youtubemodel.cpp
    youtubebackend.cpp
    videodetailsmodel.cpp
)

add_library(pmc_youtubebackend MODULE ${youtubebackend_SRCS})
set_target_properties(pmc_youtubebackend PROPERTIES PREFIX "")

target_link_libraries(pmc_youtubebackend
    plasmamediacenter
)

kcoreaddons_desktop_to_json(pmc_youtubebackend youtubebackend.desktop)

install(TARGETS pmc_youtubebackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
install(FILES youtubebackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
ecm_install_icons( ${ICON_INSTALL_DIR} )
