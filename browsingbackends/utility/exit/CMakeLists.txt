set(exitbackend_SRCS
    exitbackend.cpp
)

add_library(pmc_exit ${exitbackend_SRCS})

target_link_libraries(pmc_exit
    plasmamediacenter
    Qt5::Core
    Qt5::Gui
)

install(TARGETS pmc_exit DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pmcexit.desktop DESTINATION ${SERVICES_INSTALL_DIR})
