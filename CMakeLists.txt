project(MediaCenterComponents)

find_package(KDE4 REQUIRED)
find_package(Taglib REQUIRED)
include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${KDE4_INCLUDES}
    utils/
    widgets/
) 

add_subdirectory(dataengines)
add_subdirectory(applets)
add_subdirectory(containments)
add_subdirectory(libs)