set(localvideosbackend_SRCS
  localvideosmodel.cpp
  ../localfilesabstractbackend.cpp
  ../localfilesabstractmodel.cpp
  ../localplacesmodel.cpp
  localvideosbackend.cpp
)


add_library(pmc_localvideos MODULE ${localvideosbackend_SRCS})
set_target_properties(pmc_localvideos PROPERTIES PREFIX "")

target_link_libraries(pmc_localvideos
    plasmamediacenter
    Qt5::Core
    Qt5::Gui
    KF5::KIOWidgets
    KF5::KIOFileWidgets
    KF5::KDELibs4Support
)

kservice_desktop_to_json(pmc_localvideos localvideos.desktop)

install(TARGETS pmc_localvideos DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/browsingbackends)
install(FILES localvideos.desktop DESTINATION ${SERVICES_INSTALL_DIR})
