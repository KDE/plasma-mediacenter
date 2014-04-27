remove_definitions(-DQT_NO_STL)

add_subdirectory(mediacenter)
add_subdirectory(test)

if (DEFINED NO_LINK_TO_PLASMA)
    add_subdirectory(plasmaadapter)
endif (NO_LINK_TO_PLASMA)
