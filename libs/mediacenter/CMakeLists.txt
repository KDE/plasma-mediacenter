set (mediacenterlibs_SRCS
    mediacenter.cpp
    abstractbrowsingbackend.cpp
    backendsmodel.cpp
    playlistmodel.cpp
)

kde4_add_library(mediacenterlibs SHARED ${mediacenterlibs_SRCS})

target_link_libraries(mediacenterlibs ${KDE4_PLASMA_LIBS} ${KDE4_SOLID_LIBS} ${KDE4_PHONON_LIBS} ${KDE4_NEPOMUK_LIBS} ${TAGLIB_LIBRARIES})
install(TARGETS mediacenterlibs ${INSTALL_TARGETS_DEFAULT_ARGS})

set (mediacenterlibs_HDRS
    mediacenter.h
    abstractbrowsingbackend.h
    backendsmodel.h
    playlistmodel.h
)

install(FILES ${mediacenterlibs_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediacenter COMPONENT Devel)
install(FILES data/servicetypes/pmc_browsingbackend.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
