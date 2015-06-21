plasma_install_package(shell org.kde.plasma.mediacenter shells shell)
install(FILES plasma-mediacenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
plasma_install_package(containment org.kde.mediacentercontainment plasmoids containment)
add_subdirectory(mediabrowsers)
