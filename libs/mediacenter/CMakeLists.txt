find_package(Taglib REQUIRED)
find_package(QxOrm REQUIRED)

include_directories(
    ${TAGLIB_INCLUDES}
    ${QxOrm_INCLUDE_DIR}
)

add_definitions(-D_QX_SERIALIZE_BINARY_ENABLED -D_QX_SERIALIZE_XML_ENABLED -D_BUILDING_PMC)

set (plasmamediacenter_SRCS
    medialibrarywrappercache.cpp
    artist.cpp
    album.cpp
    media.cpp
    lastfmimagefetcher.cpp
    modelmetadata.cpp
    objectpair.cpp
    mediainfoserviceproxy.cpp
    mediainforesult.cpp
    mediainforequest.cpp
    filteredbackendsmodel.cpp
    mediacenter.cpp
    abstractbrowsingbackend.cpp
    backendsmodel.cpp
    mediainfoservice.cpp
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
)

kde4_add_library(plasmamediacenter SHARED
    ${plasmamediacenter_SRCS}
)

target_link_libraries(plasmamediacenter
    ${QT_QTDECLARATIVE_LIBRARIES}
    ${QT_QTSQL_LIBRARIES}
    ${KDE4_KDECORE_LIBRARY}
    ${KDE4_KIO_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_SOLID_LIBS}
    ${TAGLIB_LIBRARIES}
    ${NEPOMUK_CORE_LIBRARY}
    ${Boost_LIBRARIES}
    ${QxOrm_LIBRARY}
)

set_target_properties(plasmamediacenter PROPERTIES VERSION 1.2 SOVERSION 1.2)

install(TARGETS plasmamediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})

set (plasmamediacenter_HDRS
    modelmetadata.h
    mediacenter.h
    abstractbrowsingbackend.h
    backendsmodel.h
    playlistmodel.h
)

install(FILES ${plasmamediacenter_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediacenter COMPONENT Devel)
install(FILES data/servicetypes/pmc_browsingbackend.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES data/servicetypes/pmc_mediasource.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
