project(mediacontroller)

install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/plasmoids/org.kde.mediacontroller PATTERN .git EXCLUDE)

install(FILES plasma-applet-mediacontroller.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})