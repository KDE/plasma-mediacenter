include(ECMMarkAsTest)
include_directories(
    ${TAGLIB_INCLUDES}
)

find_package (PkgConfig REQUIRED)
pkg_check_modules(LIBS REQUIRED gupnp-1.0 gupnp-av-1.0 libsoup-2.4 libxml-2.0)
include_directories (${LIBS_INCLUDE_DIRS})
link_directories (${LIBS_LIBRARY_DIRS})

message(STATUS ${LIBS_LIBRARIES})

set (plasmamediacenter_SRCS
    mediaserver.cpp
    modelsinbackendmodel.cpp
    iconimageprovider.cpp
    runtimedata.cpp
    itemcache.cpp
    medialibrarywrappercache.cpp
    artist.cpp
    album.cpp
    media.cpp
    lastfmimagefetcher.cpp
    pmcmodel.cpp
    objectpair.cpp
    mediacenter.cpp
    abstractbrowsingbackend.cpp
    backendsmodel.cpp
    playlistitem.cpp
    playlistmodel.cpp
    pmccoverartprovider.cpp
    pmcmetadatamodel.cpp
    pmcimagecache.cpp
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
    localthumbnailprovider.cpp
    mpris2/mpris2.cpp
    mpris2/mediaplayer2.cpp
    mpris2/mediaplayer2player.cpp
    mpris2/mediaplayer2tracklist.cpp
)

add_library(plasmamediacenter SHARED
    ${plasmamediacenter_SRCS}
)

set(plasmamediacenter_link_libs
    Qt5::Core
    Qt5::Quick
    Qt5::Xml
    Qt5::DBus
    KF5::I18n
    KF5::ConfigCore
    KF5::GuiAddons
    KF5::CoreAddons
    KF5::Service
    KF5::KIOWidgets
    KF5::WindowSystem #needed for mpris2, not used now
    ${TAGLIB_LIBRARIES}
    -lsoup-2.4
    ${LIBS_LIBRARIES}
)

target_link_libraries(plasmamediacenter
    ${plasmamediacenter_link_libs}
)

generate_export_header(plasmamediacenter
    BASE_NAME mediacenter
    EXPORT_MACRO_NAME MEDIACENTER_EXPORT
)

add_library(plasmamediacentertest SHARED
    ${plasmamediacenter_SRCS}
)

ecm_mark_as_test(plasmamediacentertest)

set_target_properties(plasmamediacentertest
PROPERTIES COMPILE_FLAGS  -fvisibility=default
)

target_link_libraries(plasmamediacentertest
    ${plasmamediacenter_link_libs}
)


set_target_properties(plasmamediacenter PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS plasmamediacenter ${INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

install(FILES data/servicetypes/pmc_browsingbackend.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES data/servicetypes/pmc_mediasource.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

install(FILES data/xml/AVTransport1.xml DESTINATION ${DATA_INSTALL_DIR}/plasma/mediacenter)
install(FILES data/xml/ConnectionManager1.xml DESTINATION ${DATA_INSTALL_DIR}/plasma/mediacenter)
install(FILES data/xml/ContentDirectory1.xml DESTINATION ${DATA_INSTALL_DIR}/plasma/mediacenter)
#install(FILES data/xml/MediaServer1.xml DESTINATION ${DATA_INSTALL_DIR}/plasma/mediacenter)

add_subdirectory(plugin)
