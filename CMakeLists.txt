project(MediaCenterElements)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION "5.2.0")

find_package(ECM 0.0.8 REQUIRED NO_MODULE)

include(FeatureSummary)
include(WriteBasicConfigVersionFile)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ECMMarkNonGuiExecutable)

# include(KDEInstallDirs)
include(KDECMakeSettings)
# include(KDECompilerSettings)

include(ECMOptionalAddSubdirectory)
include(ECMInstallIcons)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Quick Sql Test Declarative)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)

# Load the frameworks we need
find_package(KF5 CONFIG REQUIRED
    CoreAddons Config I18n GuiAddons Service ConfigWidgets KIO Declarative Plasma)

find_package(Taglib REQUIRED)
find_package(QxOrm REQUIRED)

find_package(Boost REQUIRED COMPONENTS serialization)

include_directories(
    ${TAGLIB_INCLUDES}
    ${QxOrm_INCLUDE_DIRS}
)

# needed for backend export
find_package(KF5KDE4Support REQUIRED NO_MODULE)
find_package(Phonon4Qt5 REQUIRED)

# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE_ENABLE_EXCEPTIONS}")
set(CMAKE_AUTOMOC ON)

add_subdirectory(browsingbackends)
# add_subdirectory(plugins)
# add_subdirectory(mediaelements)
# add_subdirectory(components)
add_subdirectory(libs)
# add_subdirectory(shells)
add_subdirectory(images)
#
# include(MacroOptionalAddSubdirectory)
# macro_optional_add_subdirectory( po )

