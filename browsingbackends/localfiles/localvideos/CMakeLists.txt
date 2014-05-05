set(localvideosbackend_SRCS
  localvideosmodel.cpp
  ../localfilesabstractbackend.cpp
  ../localfilesabstractmodel.cpp
  ../localplacesmodel.cpp
  localvideosbackend.cpp
)


kde4_add_plugin(pmc_localvideos ${localvideosbackend_SRCS})

target_link_libraries(pmc_localvideos
    plasmamediacenter
    Qt5::Core
    Qt5::Gui
    KF5::KIOWidgets
    KF5::KIOFileWidgets
    KF5::KDELibs4Support
)

install(TARGETS pmc_localvideos DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localvideos.desktop DESTINATION ${SERVICES_INSTALL_DIR})
