project(mediaplayer)

install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/plasmoids/org.kde.mediaplayer PATTERN .git EXCLUDE)

install(FILES plasma-applet-mediaplayer.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})