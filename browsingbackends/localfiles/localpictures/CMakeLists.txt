set(localpicturesbackend_SRCS
    localpicturesmodel.cpp
    ../localfilesabstractbackend.cpp
    ../localfilesabstractmodel.cpp
    ../localplacesmodel.cpp
    localpicturesbackend.cpp
)

add_library(pmc_localpictures MODULE ${localpicturesbackend_SRCS})
set_target_properties(pmc_localpictures PROPERTIES PREFIX "") 

target_link_libraries(pmc_localpictures
    plasmamediacenter
    Qt5::Core
    Qt5::Gui
    KF5::KIOWidgets
    KF5::KIOFileWidgets
    KF5::KDELibs4Support
)

kservice_desktop_to_json(pmc_localpictures localpictures.desktop)

install(TARGETS pmc_localpictures DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
install(FILES localpictures.desktop DESTINATION ${SERVICES_INSTALL_DIR})
