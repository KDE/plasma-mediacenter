include_directories(../../libs ../../)

set(welcome_SRCS
mediawelcome.cpp
backendmodel.cpp
)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_mediawelcome ${welcome_SRCS})
target_link_libraries(plasma_applet_mediawelcome 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTDECLARATIVE_LIBRARY} mediacenterlibs)
 
install(TARGETS plasma_applet_mediawelcome
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-mediawelcome.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES
homeview.qml
DESTINATION ${DATA_INSTALL_DIR}/plasma-mediacenter/declarative/)
