set (mediacenterplugin_SRCS
    qmlaccess.cpp
    mediacenterplugin.cpp
)

add_library(mediacenterplugin SHARED ${mediacenterplugin_SRCS} )

target_link_libraries(mediacenterplugin
    libmedia
    Qt5::Qml
)

install(TARGETS mediacenterplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/mediacenter)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/mediacenter)
