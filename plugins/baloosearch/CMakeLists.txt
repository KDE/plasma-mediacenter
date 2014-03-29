find_package(Baloo REQUIRED)

include_directories(
  ${KDE4_INCLUDES}
  ${BALOO_INCLUDE_DIR}
)

set(baloosearch_SRCS
    baloosearchmediasource.cpp
)

qt4_automoc(${baloosearch_SRCS})

kde4_add_plugin(pmc_baloosearch ${baloosearch_SRCS})

target_link_libraries(pmc_baloosearch
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${BALOO_LIBRARIES}
)

install(TARGETS pmc_baloosearch DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES baloosearch.desktop DESTINATION ${SERVICES_INSTALL_DIR})
