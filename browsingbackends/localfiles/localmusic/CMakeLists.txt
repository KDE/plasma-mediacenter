set(localmusicbackend_SRCS
  localmusicmodel.cpp
  ../localfilesabstractbackend.cpp
  ../localfilesabstractmodel.cpp
  ../localplacesmodel.cpp
  localmusicbackend.cpp
)

add_library(pmc_localmusic ${localmusicbackend_SRCS})

target_link_libraries(pmc_localmusic
    plasmamediacenter
    Qt5::Core
    Qt5::Gui
    KF5::KIOWidgets
    KF5::KIOFileWidgets
    KF5::KDELibs4Support
    ${TAGLIB_LIBRARIES}
)

install(TARGETS pmc_localmusic DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localmusic.desktop DESTINATION ${SERVICES_INSTALL_DIR})
