set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
find_package(Taglib)
include_directories (${TAGLIB_INCLUDES})

add_subdirectory(localfiles)
add_subdirectory(metadatabackends)
add_subdirectory(cmake)
