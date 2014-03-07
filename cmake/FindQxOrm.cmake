# - Try to find QxOrm
# Once done, this will define
#
#  QxOrm_FOUND - system has QxOrm
#  QxOrm_INCLUDE_DIRS - the QxOrm include directories
#  QxOrm_LIBRARIES - link these to use QxOrm

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(QxOrm_PKGCONF QxOrm)

# Include dir
find_path(QxOrm_INCLUDE_DIR
  NAMES QxOrm.h
  PATHS ${QxOrm_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(QxOrm_LIBRARY
  NAMES QxOrmd
  PATHS ${QxOrm_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(QxOrm_PROCESS_INCLUDES QxOrm_INCLUDE_DIR QxOrm_INCLUDE_DIRS)
set(QxOrm_PROCESS_LIBS QxOrm_LIBRARY QxOrm_LIBRARIES)
libfind_process(QxOrm)

