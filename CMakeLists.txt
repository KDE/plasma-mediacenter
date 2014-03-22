project(MediaCenterElements)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

find_package(KDE4Workspace REQUIRED)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS} ${KDE4_ENABLE_EXCEPTIONS})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES}
    ${KDE4WORKSPACE_INCLUDE_DIR}
    libs/
)

add_subdirectory(browsingbackends)
add_subdirectory(plugins)
add_subdirectory(mediaelements)
add_subdirectory(components)
add_subdirectory(libs)
add_subdirectory(shells)
add_subdirectory(images)

include(MacroOptionalAddSubdirectory)
macro_optional_add_subdirectory( po )

