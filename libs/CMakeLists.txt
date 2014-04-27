find_package(Boost REQUIRED COMPONENTS serialization)

remove_definitions(-DQT_NO_STL)

add_subdirectory(mediacenter)

if (DEFINED NO_LINK_TO_PLASMA)
    add_subdirectory(plasmaadapter)
endif (NO_LINK_TO_PLASMA)
