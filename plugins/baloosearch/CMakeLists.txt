find_package(Baloo REQUIRED)
find_package(KFileMetaData REQUIRED)

include_directories(
  ${BALOO_INCLUDE_DIR}
)

set(baloosearch_SRCS
    audiosearchresulthandler.cpp
    videosearchresulthandler.cpp
    imagesearchresulthandler.cpp
    searchresulthandler.cpp
    baloosearchmediasource.cpp
)

add_library(pmc_baloosearch MODULE ${baloosearch_SRCS})
set_target_properties(pmc_baloosearch PROPERTIES PREFIX "")

target_link_libraries(pmc_baloosearch
    plasmamediacenter
    ${BALOO_CORE_LIBRARY}
    KF5::FileMetaData
)

kservice_desktop_to_json(pmc_baloosearch baloosearch.desktop)

install(TARGETS pmc_baloosearch DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/mediasources)
install(FILES baloosearch.desktop DESTINATION ${SERVICES_INSTALL_DIR})
