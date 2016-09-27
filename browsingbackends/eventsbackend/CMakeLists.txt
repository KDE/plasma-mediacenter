set(eventsbackend_SRCS
    eventsmodel.cpp
    eventsfiltermodel.cpp
    eventspicturesmodel.cpp
    ../abstractmetadatabackend.cpp
    eventsbackend.cpp
)

add_library(pmc_eventsbackend MODULE ${eventsbackend_SRCS})
set_target_properties(pmc_eventsbackend PROPERTIES PREFIX "")

target_link_libraries(pmc_eventsbackend
    KMediaCollection
)

kservice_desktop_to_json(pmc_eventsbackend eventsbackend.desktop)

install(TARGETS pmc_eventsbackend DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
install(DIRECTORY eventscomponents DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/mediacenter/elements)
