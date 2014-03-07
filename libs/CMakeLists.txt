find_package(Boost REQUIRED COMPONENTS serialization)

remove_definitions(-DQT_NO_STL)

add_subdirectory(mediacenter)
add_subdirectory(test)
