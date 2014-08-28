project(PlasmaMediaCenter)

set(PROJECT_VERSION "1.3")
set(PROJECT_VERSION_MAJOR "${PROJECT_VERSION}")

option(NO_LINK_TO_PLASMA  "Do not link to Plasma at compile-time" OFF)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
find_package(KDE4Workspace REQUIRED)

add_definitions(
    ${QT_DEFINITIONS}
    ${KDE4_DEFINITIONS}
    ${KDE4_ENABLE_EXCEPTIONS}
)

include_directories(
    ${KDE4_INCLUDES}
    ${KDE4WORKSPACE_INCLUDE_DIR}
    libs/
)

configure_file(config-plasma-mediacenter.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-plasma-mediacenter.h )

add_subdirectory(libs)

add_subdirectory(plugins)
add_subdirectory(browsingbackends)

add_subdirectory(mediaelements)
add_subdirectory(components)
add_subdirectory(shells)
add_subdirectory(images)

include(MacroOptionalAddSubdirectory)
macro_optional_add_subdirectory( po )
