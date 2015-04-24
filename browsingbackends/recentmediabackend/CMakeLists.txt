set(recentmediabackend_SRCS
    recentmediabackend.cpp
    recentmediamodel.cpp
)

add_library(pmc_recentmediabackend MODULE ${recentmediabackend_SRCS})
set_target_properties(pmc_recentmediabackend PROPERTIES PREFIX "")

target_link_libraries(pmc_recentmediabackend
    plasmamediacenter
    KF5::ActivitiesExperimentalStats
)

kcoreaddons_desktop_to_json(pmc_recentmediabackend recentmediabackend.desktop)

install(TARGETS pmc_recentmediabackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
