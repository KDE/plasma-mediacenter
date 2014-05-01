add_subdirectory(mediacenter)
add_subdirectory(test)

if (NO_LINK_TO_PLASMA)
    add_subdirectory(plasmaadapter)
endif (NO_LINK_TO_PLASMA)

