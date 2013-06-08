set(picasabackend_SRCS
picasabackend.cpp
picasamodel.cpp
)

qt4_automoc(${picasabackend_SRCS})

kde4_add_plugin(pmc_picasabackend ${picasabackend_SRCS})

target_link_libraries(pmc_picasabackend
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_QTXML_LIBRARY}
)

install(TARGETS pmc_picasabackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES picasabackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(DIRECTORY picasacomponents DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/mediacenter/elements)
