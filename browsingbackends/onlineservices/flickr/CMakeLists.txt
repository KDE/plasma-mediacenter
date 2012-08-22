set(flickrbackend_SRCS
    flickrmodel.cpp
    flickrbackend.cpp
)

qt4_automoc(${flickrbackend_SRCS})

kde4_add_plugin(pmc_flickrbackend ${flickrbackend_SRCS})

target_link_libraries(pmc_flickrbackend
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_QTXML_LIBRARY}
)

install(TARGETS pmc_flickrbackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES flickrbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
