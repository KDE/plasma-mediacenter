find_package(Taglib REQUIRED)

include_directories(
    ${TAGLIB_INCLUDES}
)

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
)

add_library(plasmamediacenter SHARED
    ${plasmamediacenter_SRCS}
)

target_link_libraries(plasmamediacenter
    Qt5::Core
    Qt5::Declarative
    KF5::Solid
    KF5::GuiAddons
    KF5::CoreAddons
    KF5::KDELibs4Support
    ${TAGLIB_LIBRARIES}
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

add_subdirectory(plugin)
##############################          TESTS           ##############################

    function(pmc_test TESTNAME)
        add_executable(${TESTNAME} ${plasmamediacenter_SRCS} test/${TESTNAME}.cpp test/fakemediavalidator.cpp)
        add_test(pmc-libs-${TESTNAME} ${TESTNAME})
        target_link_libraries(${TESTNAME}
            Qt5::Core
            Qt5::Declarative
            Qt5::Test
        )
    endfunction(pmc_test)

    pmc_test(medialibrarytest)
    pmc_test(singletonfactorytest)
    pmc_test(lastfmimagefetchertest)
    pmc_test(pmcmediatest)
    pmc_test(mediatest)
    pmc_test(mediacentertest)

######################################################################################
