set(fullscreenbackend_SRCS
  fullscreenbackend.cpp
)

add_library(pmc_fullscreen ${fullscreenbackend_SRCS})

target_link_libraries(pmc_fullscreen
    plasmamediacenter
    Qt5::Core
    Qt5::Gui
)

install(TARGETS pmc_fullscreen DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pmcfullscreen.desktop DESTINATION ${SERVICES_INSTALL_DIR})
