if(KF5Baloo_FOUND)
    add_subdirectory(baloosearch)
else()
    add_subdirectory(filesystemsearch)
endif()

add_subdirectory(upnp)