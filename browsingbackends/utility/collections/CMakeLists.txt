set(collectionsbackend_SRCS
  collectionsbackend.cpp
  collectionsmodel.cpp
)

qt4_automoc(${collectionsbackend_SRCS})

kde4_add_plugin(pmc_collections ${collectionsbackend_SRCS})

target_link_libraries(pmc_collections
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    ${QT_QTDECLARATIVE_LIBRARIES}
)

install(TARGETS pmc_collections DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pmccollections.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(DIRECTORY add DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/mediacenter/elements)