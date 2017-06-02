include(ECMMarkAsTest)
include_directories(
    ${TAGLIB_INCLUDES}
)
set (plasmamediacenter_SRCS
    iconimageprovider.cpp
    runtimedata.cpp
    itemcache.cpp
    medialibrarywrappercache.cpp
    artist.cpp
    album.cpp
    media.cpp
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
    abstractdatasource.cpp
    datasourcesloader.cpp
    singletonfactory.cpp
    mediavalidator.cpp
    settings.cpp
    localthumbnailprovider.cpp
    mediabrowserloader.cpp
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
    KF5::Activities
    KF5::KIOWidgets
    KF5::Plasma
    ${TAGLIB_LIBRARIES}
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
install(FILES data/servicetypes/pmc_datasource.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

add_subdirectory(plugin)
