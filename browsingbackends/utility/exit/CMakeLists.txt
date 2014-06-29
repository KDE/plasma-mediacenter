set(exitbackend_SRCS
    exitbackend.cpp
)

qt4_automoc(${exitbackend_SRCS})

kde4_add_plugin(pmc_exit ${exitbackend_SRCS})

target_link_libraries(pmc_exit
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
)

install(TARGETS pmc_exit DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pmcexit.desktop DESTINATION ${SERVICES_INSTALL_DIR})
