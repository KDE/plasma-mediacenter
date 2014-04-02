set(flickrbackend_SRCS
    flickrmodel.cpp
    flickrbackend.cpp
)

add_library(pmc_flickrbackend ${flickrbackend_SRCS})

target_link_libraries(pmc_flickrbackend
    plasmamediacenter
)

install(TARGETS pmc_flickrbackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES flickrbackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
