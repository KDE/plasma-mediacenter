project(MediaCenterComponents)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES}
    libs/
)

add_subdirectory(browsingbackends)
add_subdirectory(components)
add_subdirectory(libs)
add_subdirectory(shells)
