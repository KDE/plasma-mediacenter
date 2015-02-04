set(upnp_SRCS
    upnpmediasource.cpp
    mediahandler.cpp
)
FIND_PACKAGE(Qt5Widgets REQUIRED)
add_library(pmc_upnp MODULE ${upnp_SRCS})
set_target_properties(pmc_upnp PROPERTIES PREFIX "")

find_package (PkgConfig REQUIRED)
pkg_check_modules(LIBS REQUIRED gupnp-1.0 gtk+-2.0 gssdp-1.0 gupnp-av-1.0 libxml-2.0)
include_directories (${LIBS_INCLUDE_DIRS})
link_directories (${LIBS_LIBRARY_DIRS})

target_link_libraries(pmc_upnp
    plasmamediacenter
    ${LIBS_LIBRARIES}
)

kcoreaddons_desktop_to_json(pmc_upnp upnp.desktop)

install(TARGETS pmc_upnp DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/mediasources)
install(FILES upnp.desktop DESTINATION ${SERVICES_INSTALL_DIR})

