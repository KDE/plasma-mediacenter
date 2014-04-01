set(filesystemsearch_SRCS
    filesystemsearchmediasource.cpp
)

qt4_automoc(${filesystemsearch_SRCS})

kde4_add_plugin(pmc_filesystemsearch ${filesystemsearch_SRCS})

target_link_libraries(pmc_filesystemsearch
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${QT_QTGUI_LIBRARIES}
)

install(TARGETS pmc_filesystemsearch DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES filesystemsearch.desktop DESTINATION ${SERVICES_INSTALL_DIR})
