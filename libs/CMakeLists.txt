project(KMediaCollection)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.5.0")
set (KF5_MIN_VERSION "5.20.0")

set(PROJECT_VERSION "0.90")
set(PROJECT_VERSION_MAJOR "0")

include(FeatureSummary)
find_package(ECM 5.26.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(WriteBasicConfigVersionFile)
include(GenerateExportHeader)
include(ECMMarkNonGuiExecutable)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

include(ECMOptionalAddSubdirectory)

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

find_package(Qt5 CONFIG REQUIRED COMPONENTS Core DBus Xml Quick Test)

find_package(Qt5Multimedia QUIET)
set_package_properties(Qt5Multimedia PROPERTIES
        PURPOSE "Qt5Multimedia QML bindings are required at runtime to play media files"
        TYPE RUNTIME)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

# Load the frameworks we need
find_package(KF5 ${KF5_MIN_VERSION} COMPONENTS REQUIRED
    CoreAddons Config I18n GuiAddons Service KIO Declarative Activities Plasma PlasmaQuick)

find_package(KF5Baloo)
add_feature_info("KF5Baloo" KF5Baloo_FOUND "Required for Baloo integration")
find_package(Taglib REQUIRED)

include_directories(
    ${TAGLIB_INCLUDES}
)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KMEDIACOLLECTION
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kmediacollection_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KMediaCollectionConfigVersion.cmake"
    SOVERSION 5)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE_ENABLE_EXCEPTIONS}")
set(CMAKE_AUTOMOC ON)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(autotests)
#
# include(MacroOptionalAddSubdirectory)
# macro_optional_add_subdirectory( po )

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KMediaCollection")

include(ECMPackageConfigHelpers)

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KMediaCollectionConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KMediaCollectionConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kmediacollection_version.h  
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}
    COMPONENT Devel)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KMediaCollectionConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KMediaCollectionConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel)

install(EXPORT KMediaCollectionTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KMediaCollectionTargets.cmake)
#        NAMESPACE KF5::)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
