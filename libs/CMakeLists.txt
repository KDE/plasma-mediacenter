add_subdirectory(mediacenter)

if (DEFINED NO_LINK_TO_PLASMA)
    add_subdirectory(plasmaadapter)
endif (NO_LINK_TO_PLASMA)
