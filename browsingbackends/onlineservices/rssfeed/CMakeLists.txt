set(rssbackend_SRCS
rssmodel.cpp
proxymodel.cpp
rssbackend.cpp
)

find_package (KdepimLibs REQUIRED)

include_directories( ${KDE4_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIRS} ${QT_INCLUDES} )

qt4_automoc(${rssbackend_SRCS})

kde4_add_plugin(pmc_rssbackend ${rssbackend_SRCS})

target_link_libraries(pmc_rssbackend
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${KDEPIMLIBS_KRSS_LIBS}
    ${KDEPIMLIBS_AKONADI_LIBS}
    ${KDEPIMLIBS_AKONADI_KMIME_LIBS}
    ${KDEPIMLIBS_KMIME_LIBS}
)

install(TARGETS pmc_rssbackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rssbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(DIRECTORY rsscomponents DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/mediacentercomponents)