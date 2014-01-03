find_package(Taglib REQUIRED)

include(ODB)

include_directories(
    ${TAGLIB_INCLUDES}
)

set (plasmamediacenter_SRCS
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
    metadataupdater.cpp
    playlistitem.cpp
    playlistmodel.cpp
    pmccoverartprovider.cpp
    pmcmetadatamodel.cpp
    pmcimagecache.cpp
    pmcimageprovider.cpp
    subtitleprovider.cpp
    filterplaylistmodel.cpp
    multipleplaylistmodel.cpp
)

set (plasmamediacenter_ODB_SRCS
    media_odb.cpp
)

kde4_add_library(plasmamediacenter SHARED 
    ${plasmamediacenter_SRCS}
    ${plasmamediacenter_ODB_SRCS}
)

set(ODB_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/media.h)

odb_compile(HEADER ${ODB_HEADERS} DB sqlite
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA)

target_link_libraries(plasmamediacenter
    ${QT_QTDECLARATIVE_LIBRARIES}
    ${KDE4_KDECORE_LIBRARY}
    ${KDE4_KIO_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_SOLID_LIBS}
    ${TAGLIB_LIBRARIES}
    ${NEPOMUK_CORE_LIBRARY}
    -lodb -lodb-sqlite
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
