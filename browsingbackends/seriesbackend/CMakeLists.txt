set(seriesbackend_SRCS clusterer.cpp cluster.cpp seriesmodel.cpp
  seriesbackend.cpp
)

qt4_automoc(${seriesbackend_SRCS})

kde4_add_plugin(pmc_series ${seriesbackend_SRCS})

target_link_libraries(pmc_series
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    ${QT_QTDECLARATIVE_LIBRARIES}
)

install(TARGETS pmc_series DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pmcseries.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if (KDE4_BUILD_TESTS)
    add_subdirectory(test)
endif (KDE4_BUILD_TESTS)
