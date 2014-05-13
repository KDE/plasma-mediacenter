add_subdirectory(mediacenter)

if (KDE4_BUILD_TESTS)
    add_subdirectory(test)
endif (KDE4_BUILD_TESTS)

if (NO_LINK_TO_PLASMA)
    add_subdirectory(plasmaadapter)
endif (NO_LINK_TO_PLASMA)

