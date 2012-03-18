project(mediainfobar)

install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/plasmoids/org.kde.mediainfobar PATTERN .git EXCLUDE)

install(FILES plasma-applet-mediainfobar.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})