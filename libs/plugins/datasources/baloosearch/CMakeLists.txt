if(NOT KF5Baloo_FOUND)
    return()
endif()
find_package(KF5FileMetaData REQUIRED)

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
    KMediaCollection
    Qt5::DBus
    KF5::Baloo
    KF5::FileMetaData
)

kcoreaddons_desktop_to_json(pmc_baloosearch baloosearch.desktop)

install(TARGETS pmc_baloosearch DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacollection/datasources)
