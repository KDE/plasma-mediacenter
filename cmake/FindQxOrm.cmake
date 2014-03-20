# - Try to find QxOrm
# Once done, this will define
#
#  QxOrm_FOUND - system has QxOrm
#  QxOrm_INCLUDE_DIRS - the QxOrm include directories
#  QxOrm_LIBRARIES - link these to use QxOrm

# Include dir
find_path(QxOrm_INCLUDE_DIRS
  NAMES QxOrm.h
)

# Finally the library itself
find_library(QxOrm_LIBRARIES
  NAMES QxOrmd QxOrm
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(QxOrm DEFAULT_MSG
                                                          QxOrm_LIBRARIES
                                                          QxOrm_INCLUDE_DIRS
)

mark_as_advanced(QxOrm_INCLUDE_DIRS QxOrm_LIBRARIES)
