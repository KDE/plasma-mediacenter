find_package(SharedDesktopOntologies REQUIRED)
find_package(Soprano REQUIRED)
find_package(Nepomuk REQUIRED)

include(SopranoAddOntology)

add_subdirectory(localvideos)
add_subdirectory(localpictures)
add_subdirectory(localmusic)

install(DIRECTORY localfilescomponents DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/plasma/mediacentercomponents)
