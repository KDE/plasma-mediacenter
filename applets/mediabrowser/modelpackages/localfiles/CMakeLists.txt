set(localfilespackage_SRCS
    localfilespackage.cpp
)

kde4_add_plugin(mc_localfiles ${localfilespackage_SRCS})
target_link_libraries(mc_localfiles mediabrowserlibs ${KDE4_KDECORE_LIBS})

install(TARGETS mc_localfiles DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localfiles.desktop DESTINATION ${SERVICES_INSTALL_DIR})