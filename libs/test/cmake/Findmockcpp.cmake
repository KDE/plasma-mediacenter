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

# - Try to find mockcpp
# Once done, this will define
#
#  mockcpp_FOUND - system has mockcpp
#  mockcpp_INCLUDE_DIRS - the mockcpp include directories
#  mockcpp_LIBRARIES - link these to use mockcpp

# Include dir
find_path(mockcpp_INCLUDE_DIRS
  NAMES mockcpp/mockcpp.h
)

# Finally the library itself
find_library(mockcpp_LIBRARIES
  NAMES mockcpp
)

if(mockcpp_LIBRARIES)
  set(mockcpp_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(mockcpp DEFAULT_MSG
    mockcpp_LIBRARIES
    mockcpp_INCLUDE_DIRS
)

mark_as_advanced(mockcpp_INCLUDE_DIRS mockcpp_LIBRARIES)
