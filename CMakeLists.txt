project(MediaCenterElements)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

find_package(KDE4Workspace REQUIRED)
find_package(NepomukCore REQUIRED)
find_package(Soprano)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES}
    ${KDE4WORKSPACE_INCLUDE_DIR}
    ${NEPOMUK_CORE_INCLUDE_DIR}
    ${SOPRANO_INCLUDE_DIR}
    libs/
)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

add_subdirectory(browsingbackends)
add_subdirectory(mediaelements)
add_subdirectory(libs)
add_subdirectory(shells)

message("*** QtMultimediaKit (included in Qt-Mobility) is required at runtime to play multimedia.")
message("*** You can download it from http://get.qt.nokia.com/qt/add-ons/qt-mobility-opensource-src-1.2.0.zip .")

