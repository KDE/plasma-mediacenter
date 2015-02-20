project(PlasmaMediaCenter)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.3.0")
set (KF5_MIN_VERSION "5.5.0")

set(PROJECT_VERSION "2.0.0")
set(PROJECT_VERSION_MAJOR "2")

find_package(ECM 0.0.8 REQUIRED NO_MODULE)
include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(GenerateExportHeader)
# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ECMMarkNonGuiExecutable)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

include(ECMOptionalAddSubdirectory)
include(ECMInstallIcons)

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

find_package(Qt5 CONFIG REQUIRED COMPONENTS Core DBus Xml Quick Sql Test)

find_package(Qt5Multimedia)
add_feature_info("Qt5Multimedia" Qt5Multimedia_FOUND "Qt5Multimedia QML bindings are required at runtime to play media files")

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)

# Load the frameworks we need
find_package(KF5 ${KF5_MIN_VERSION} COMPONENTS REQUIRED
    CoreAddons Config I18n GuiAddons Service ConfigWidgets KIO Declarative Plasma PlasmaQuick)

find_package(KF5Baloo 5.1.90)
find_package(Taglib REQUIRED)

include_directories(
    ${TAGLIB_INCLUDES}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    libs/
)

# if plasma-workspace is installed in different prefix then pmc then it will not work

set(STARTKDE_BINARY_DIR ${CMAKE_INSTALL_FULL_BINDIR})

configure_file(plasma-mediacenter.desktop.cmake ${CMAKE_CURRENT_BINARY_DIR}/plasma-mediacenter.desktop)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/plasma-mediacenter.desktop
        DESTINATION ${DATA_INSTALL_DIR}/xsessions
)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE_ENABLE_EXCEPTIONS}")
set(CMAKE_AUTOMOC ON)

add_subdirectory(browsingbackends)
add_subdirectory(mediasources)
#add_subdirectory(onlineservices)
add_subdirectory(mediaelements)
add_subdirectory(components)
add_subdirectory(libs)
add_subdirectory(shells)
add_subdirectory(images)
add_subdirectory(tests)
add_subdirectory(autotests)
#
# include(MacroOptionalAddSubdirectory)
# macro_optional_add_subdirectory( po )

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
