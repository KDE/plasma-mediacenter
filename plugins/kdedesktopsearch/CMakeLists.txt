find_package(NepomukCore REQUIRED)

include_directories(
    ${NEPOMUK_CORE_INCLUDE_DIR}
    ${SOPRANO_INCLUDE_DIR}
)

set(kdedesktopsearch_SRCS
    kdemetadatamediasource.cpp
)

qt4_automoc(${kdedesktopsearch_SRCS})

kde4_add_plugin(pmc_kdedesktopsearch ${kdedesktopsearch_SRCS})

target_link_libraries(pmc_kdedesktopsearch
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARIES}
    ${NEPOMUK_CORE_LIBRARY}
)

install(TARGETS pmc_kdedesktopsearch DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kdedesktopsearch.desktop DESTINATION ${SERVICES_INSTALL_DIR})
