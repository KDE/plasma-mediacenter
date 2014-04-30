find_package(Baloo REQUIRED)

include_directories(
  ${BALOO_INCLUDE_DIR}
)

set(baloosearch_SRCS
    baloosearchmediasource.cpp
)

add_library(pmc_baloosearch ${baloosearch_SRCS})

target_link_libraries(pmc_baloosearch
    plasmamediacenter
    ${BALOO_CORE_LIBRARY}
)

install(TARGETS pmc_baloosearch DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES baloosearch.desktop DESTINATION ${SERVICES_INSTALL_DIR})
