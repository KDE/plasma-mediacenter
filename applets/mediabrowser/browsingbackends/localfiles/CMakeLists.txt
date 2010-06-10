find_package(SharedDesktopOntologies REQUIRED)
find_package(Soprano REQUIRED)
find_package(Nepomuk REQUIRED)

include(SopranoAddOntology)


add_subdirectory(localvideos)
add_subdirectory(localpictures)
add_subdirectory(localmusic)
