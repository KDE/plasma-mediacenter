# - Try to find the Taglib library
# Once done this will define
#
#  TAGLIB_FOUND - system has the taglib library
#  TAGLIB_CFLAGS - the taglib cflags
#  TAGLIB_LIBRARIES - The libraries needed to use taglib

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

if(NOT TAGLIB_MIN_VERSION)
  set(TAGLIB_MIN_VERSION "1.4")
endif(NOT TAGLIB_MIN_VERSION)

if(NOT WIN32)
    find_program(TAGLIBCONFIG_EXECUTABLE NAMES taglib-config PATHS
       ${BIN_INSTALL_DIR}
    )
endif(NOT WIN32)

#reset vars
set(TAGLIB_LIBRARIES)
set(TAGLIB_CFLAGS)

# if taglib-config has been found
if(TAGLIBCONFIG_EXECUTABLE)

  exec_program(${TAGLIBCONFIG_EXECUTABLE} ARGS --version RETURN_VALUE _return_VALUE OUTPUT_VARIABLE TAGLIB_VERSION)

  if(TAGLIB_VERSION VERSION_LESS "${TAGLIB_MIN_VERSION}")
     message(STATUS "TagLib version not found: version searched :${TAGLIB_MIN_VERSION}, found ${TAGLIB_VERSION}")
     set(TAGLIB_FOUND FALSE)
  else(TAGLIB_VERSION VERSION_LESS "${TAGLIB_MIN_VERSION}")

     exec_program(${TAGLIBCONFIG_EXECUTABLE} ARGS --libs RETURN_VALUE _return_VALUE OUTPUT_VARIABLE TAGLIB_LIBRARIES)

     exec_program(${TAGLIBCONFIG_EXECUTABLE} ARGS --cflags RETURN_VALUE _return_VALUE OUTPUT_VARIABLE TAGLIB_CFLAGS)

     if(TAGLIB_LIBRARIES AND TAGLIB_CFLAGS)
        set(TAGLIB_FOUND TRUE)
        message(STATUS "Found taglib: ${TAGLIB_LIBRARIES}")
     endif(TAGLIB_LIBRARIES AND TAGLIB_CFLAGS)
     string(REGEX REPLACE " *-I" ";" TAGLIB_INCLUDES "${TAGLIB_CFLAGS}")
  endif(TAGLIB_VERSION VERSION_LESS "${TAGLIB_MIN_VERSION}")
  mark_as_advanced(TAGLIB_CFLAGS TAGLIB_LIBRARIES TAGLIB_INCLUDES)

else(TAGLIBCONFIG_EXECUTABLE)

  include(FindPackageHandleStandardArgs)

  find_path(TAGLIB_INCLUDES
    NAMES
    tag.h
    PATH_SUFFIXES taglib
  )

  find_library(TAGLIB_LIBRARIES
    NAMES tag
  )
  
  find_package_handle_standard_args(Taglib DEFAULT_MSG 
                                    TAGLIB_INCLUDES TAGLIB_LIBRARIES)
endif(TAGLIBCONFIG_EXECUTABLE)


if(TAGLIB_FOUND)
  if(NOT Taglib_FIND_QUIETLY AND TAGLIBCONFIG_EXECUTABLE)
    message(STATUS "Taglib found: ${TAGLIB_LIBRARIES}")
  endif(NOT Taglib_FIND_QUIETLY AND TAGLIBCONFIG_EXECUTABLE)
else(TAGLIB_FOUND)
  if(Taglib_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find Taglib")
  endif(Taglib_FIND_REQUIRED)
endif(TAGLIB_FOUND)

