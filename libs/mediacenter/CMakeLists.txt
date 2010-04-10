include_directories(${CMAKE_CURRENT_SOURCE_DIR}
)
set (mediacenterlibs_SRCS
browser.cpp
playbackcontrol.cpp
playlist.cpp
player.cpp
mediacenter.cpp
mediacenterstate.cpp
picturestate.cpp
videostate.cpp
musicstate.cpp
medialayout.cpp
private/mediahandler.cpp
)

kde4_add_library(mediacenterlibs SHARED ${mediacenterlibs_SRCS})

target_link_libraries(mediacenterlibs ${KDE4_PLASMA_LIBS} ${KDE4_SOLID_LIBS} ${KDE4_PHONON_LIBS})
install(TARGETS mediacenterlibs ${INSTALL_TARGETS_DEFAULT_ARGS})

set (mediacenterlibs_HDRS
    mediacenter.h
    browser.h
    playbackcontrol.h
    playlist.h
    player.h
    mediacenterstate.h
    picturestate.h
    videostate.h
    musicstate.h
    medialayout.h
)

install (FILES ${mediacenterlibs_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediacenter COMPONENT Devel)
