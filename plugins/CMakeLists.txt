find_package(Baloo QUIET)
if(Baloo_FOUND)
    message(STATUS "********** Found Baloo, building Baloo Search plugin")
    add_subdirectory(baloosearch)
else(Baloo_FOUND)
    find_package(NepomukCore QUIET)

    if(NepomukCore_FOUND)
        message(STATUS "********** Found Nepomuk, building Nepomuk Search plugin")
        add_subdirectory(kdedesktopsearch)
    else(NepomukCore_FOUND)
        message(STATUS "********** Neither Baloo nor Nepomuk was found, install one of them to get more functionality")

        if(USE_FILESYSTEM_MEDIA_SOURCE)
            message(STATUS "********** Building the limited Filesystem Search plugin")
            add_subdirectory(filesystemsearch)
        else(USE_FILESYSTEM_MEDIA_SOURCE)
            message(STATUS "********** Because of this, no media sources are being built. Only local browsing will be available")
            message(STATUS "********** If you not on KDE or cannot install Baloo or Nepomuk, set USE_FILESYSTEM_MEDIA_SOURCE=ON to cmake to build the limited Filesystem Search plugin")
        endif(USE_FILESYSTEM_MEDIA_SOURCE)

    endif(NepomukCore_FOUND)

endif(Baloo_FOUND)
