set(fullscreenbackend_SRCS
  fullscreenbackend.cpp
)

qt4_automoc(${picasabackend_SRCS})

kde4_add_plugin(pmc_fullscreen ${fullscreenbackend_SRCS})

target_link_libraries(pmc_fullscreen
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    ${QT_QTDECLARATIVE_LIBRARIES}
)

install(TARGETS pmc_fullscreen DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pmcfullscreen.desktop DESTINATION ${SERVICES_INSTALL_DIR})
