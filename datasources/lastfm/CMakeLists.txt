set(lastfm_SRCS
    lastfmimagefetcher.cpp
)

add_library(pmc_lastfm MODULE ${lastfm_SRCS})
set_target_properties(pmc_lastfm PROPERTIES PREFIX "")

target_link_libraries(pmc_lastfm
    plasmamediacenter
)

kcoreaddons_desktop_to_json(pmc_lastfm lastfmimagefetcher.desktop)

install(TARGETS pmc_lastfm DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/datasources)
