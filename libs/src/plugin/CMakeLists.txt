set (kmediacollectionplugin_SRCS
    qmlaccess.cpp
    kmediacollectionplugin.cpp
)

add_library(KMediaCollectionPlugin SHARED ${kmediacollectionplugin_SRCS} )

target_link_libraries(KMediaCollectionPlugin
    KMediaCollection
    Qt5::Qml
)

install(TARGETS KMediaCollectionPlugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/mediacollection)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/mediacollection)
