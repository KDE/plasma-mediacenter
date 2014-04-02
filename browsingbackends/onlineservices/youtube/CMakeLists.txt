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

add_library(pmc_youtubebackend ${youtubebackend_SRCS})

target_link_libraries(pmc_youtubebackend
    plasmamediacenter
)

install(TARGETS pmc_youtubebackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES youtubebackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
kde4_install_icons( ${ICON_INSTALL_DIR} )
