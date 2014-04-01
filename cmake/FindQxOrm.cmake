#   Copyright 2014 Shantanu Tushar <shantanu@kde.org>
#
#   This library is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Lesser General Public
#   License as published by the Free Software Foundation; either
#   version 2.1 of the License, or (at your option) any later version.
#
#   This library is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Lesser General Public License for more details.

#   You should have received a copy of the GNU Lesser General Public
#   License along with this library.  If not, see <http://www.gnu.org/licenses/>.

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
