find_package(Taglib REQUIRED)
find_package(QxOrm REQUIRED)

include_directories(
    ${TAGLIB_INCLUDES}
    ${QxOrm_INCLUDE_DIRS}
)

add_definitions(
    -D_QX_SERIALIZE_BINARY_ENABLED
    -D_QX_SERIALIZE_XML_ENABLED
    -D_BUILDING_PMC
)

# we have both version available
if(QxOrm_RELEASE_FOUND AND QxOrm_DEBUG_FOUND)
    # We are building in debug mode
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DebugFull")
        add_definitions(
            -D_QX_MODE_DEBUG
        )
    else()
    # We are building in release mode
        add_definitions(
            -D_QX_MODE_RELEASE
        )
    endif()

# We found only release version
elseif(QxOrm_RELEASE_FOUND)
    add_definitions(
        -D_QX_MODE_RELEASE
    )

# We found only debug version
elseif(QxOrm_DEBUG_FOUND)
    add_definitions(
        -D_QX_MODE_DEBUG
    )
endif()

set (plasmamediacenter_SRCS
    medialibrarywrappercache.cpp
    artist.cpp
    album.cpp
    media.cpp
    lastfmimagefetcher.cpp
    modelmetadata.cpp
    objectpair.cpp
    filteredbackendsmodel.cpp
    mediacenter.cpp
    abstractbrowsingbackend.cpp
    backendsmodel.cpp
    playlistitem.cpp
    playlistmodel.cpp
    pmccoverartprovider.cpp
    pmcmetadatamodel.cpp
    pmcimagecache.cpp
    pmcimageprovider.cpp
    subtitleprovider.cpp
    filterplaylistmodel.cpp
    multipleplaylistmodel.cpp
    medialibrary.cpp
    pmcmedia.cpp
    pmcalbum.cpp
    pmcartist.cpp
    filtermediamodel.cpp
    abstractmediasource.cpp
    mediasourcesloader.cpp
    singletonfactory.cpp
    mediavalidator.cpp
    settings.cpp
    ../mpris2/mpris2.cpp
    ../mpris2/mediaplayer2.cpp
    ../mpris2/mediaplayer2player.cpp
)

kde4_add_library(plasmamediacenter SHARED
    ${plasmamediacenter_SRCS}
)

target_link_libraries(plasmamediacenter
    ${QT_QTDECLARATIVE_LIBRARIES}
    ${QT_QTSQL_LIBRARIES}
    ${KDE4_KDECORE_LIBRARY}
    ${KDE4_KIO_LIBS}
    ${KDE4_SOLID_LIBS}
    ${TAGLIB_LIBRARIES}
    ${NEPOMUK_CORE_LIBRARY}
    ${Boost_LIBRARIES}
    ${QxOrm_LIBRARIES}
)

set_target_properties(plasmamediacenter PROPERTIES VERSION 1.2 SOVERSION 1.2)

install(TARGETS plasmamediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})

set (plasmamediacenter_HDRS
    abstractbrowsingbackend.h
    abstractmediasource.h
    modelmetadata.h
    mediacenter.h
    medialibrary.h
    pmcmedia.h
    pmcalbum.h
    pmcartist.h
    singletonfactory.h
)

install(FILES ${plasmamediacenter_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediacenter COMPONENT Devel)
install(FILES data/servicetypes/pmc_browsingbackend.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES data/servicetypes/pmc_mediasource.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
