include(ECMMarkAsTest)
include_directories(
    ${TAGLIB_INCLUDES}
)
set (kmediacollection_SRCS
    iconimageprovider.cpp
    runtimedata.cpp
    itemcache.cpp
    medialibrarywrappercache.cpp
    artist.cpp
    album.cpp
    media.cpp
    pmcmodel.cpp
    objectpair.cpp
    kmediacollection.cpp
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

add_library(KMediaCollection SHARED
    ${kmediacollection_SRCS}
)


set(kmediacollection_link_libs
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

target_link_libraries(KMediaCollection
    ${kmediacollection_link_libs}
)


generate_export_header(KMediaCollection
    EXPORT_MACRO_NAME KMEDIACOLLECTION_EXPORT
)

ecm_generate_headers(kmediacollection_HEADERS
    HEADER_NAMES
    AbstractBrowsingBackend
    BackendsModel
    IconImageProvider
    MediaBrowserLoader
    MediaValidator
    PlaylistModel
    PMCImageCache
    RuntimeData
    AbstractDataSource
    DataSourcesLoader
    ItemCache
    KMediaCollection
    MultiplePlaylistModel
    PMCAlbum
    PMCMedia
    Settings
    Album
    FilterMediaModel
    LocalThumbnailProvider
    MediaLibrary
    ObjectPair
    PMCArtist
    PMCMetadataModel
    SingletonFactory
    Artist
    FilterPlaylistModel
    Media
    MediaLibraryWrapperCache
    PlaylistItem
    PMCCoverArtProvider
    PMCModel
    SubtitleProvider

    REQUIRED_HEADERS kmediacollection_HEADERS
)

target_include_directories(KMediaCollection
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KMediaCollection>"
)
install(TARGETS KMediaCollection
    EXPORT KMediaCollection
    ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kmediacollection_export.h
    ${kmediacollection_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KMediaCollection
    COMPONENT Devel)

add_library(KMediaCollectionTest SHARED
    ${kmediacollection_SRCS}
)

ecm_mark_as_test(KMediaCollectionTest)

set_target_properties(KMediaCollectionTest
PROPERTIES COMPILE_FLAGS  -fvisibility=default
)

target_link_libraries(KMediaCollectionTest
    ${kmediacollection_link_libs}
)

install(TARGETS KMediaCollection
    EXPORT KMediaCollectionTargets
    ${INSTALL_TARGETS_DEFAULT_ARGS})

set_target_properties(KMediaCollection PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS KMediaCollection ${INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

install(FILES data/servicetypes/pmc_browsingbackend.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES data/servicetypes/pmc_datasource.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KMediaCollection LIB_NAME KMediaCollection DEPS "core" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KMediaCollection)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

add_subdirectory(plugin)
