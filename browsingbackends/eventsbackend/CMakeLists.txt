set(eventsbackend_SRCS
    eventsmodel.cpp
    eventsfiltermodel.cpp
    eventspicturesmodel.cpp
    ../abstractmetadatabackend.cpp
    eventsbackend.cpp
)

qt4_automoc(${eventsbackend_SRCS})

kde4_add_plugin(pmc_eventsbackend ${eventsbackend_SRCS})

target_link_libraries(pmc_eventsbackend
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
)

install(TARGETS pmc_eventsbackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES eventsbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(DIRECTORY eventscomponents DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/mediacenter/elements)
