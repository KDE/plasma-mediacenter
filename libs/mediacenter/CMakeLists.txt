set (plasmamediacenter_SRCS
    filteredbackendsmodel.cpp
    mediacenter.cpp
    abstractbrowsingbackend.cpp
    backendsmodel.cpp
    metadataupdater.cpp
    playlistmodel.cpp
    pmcmetadatamodel.cpp
    pmcimagecache.cpp
    pmcimageprovider.cpp
)

kde4_add_library(plasmamediacenter SHARED ${plasmamediacenter_SRCS})

target_link_libraries(plasmamediacenter
    ${QT_QTDECLARATIVE_LIBRARIES}
    ${KDE4_KDECORE_LIBRARY}
    ${KDE4_KIO_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_SOLID_LIBS}
    ${TAGLIB_LIBRARIES}
    ${NEPOMUK_LIBRARIES}
    ${NEPOMUK_QUERY_LIBRARIES})
set_target_properties(plasmamediacenter PROPERTIES VERSION 0.9 SOVERSION 0.9)

install(TARGETS plasmamediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})

set (plasmamediacenter_HDRS
    mediacenter.h
    abstractbrowsingbackend.h
    backendsmodel.h
    playlistmodel.h
)

install(FILES ${plasmamediacenter_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediacenter COMPONENT Devel)
install(FILES data/servicetypes/pmc_browsingbackend.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
