plasma_install_package(shell org.kde.plasma.mediacenter shells shell)
install(FILES plasma-mediacenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
add_subdirectory(containment)
add_subdirectory(mediabrowsers)
