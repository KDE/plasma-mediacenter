#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (mediacenterlibs_SRCS
gesturerecognizer.cpp
playbackgesture.cpp
browsergesture.cpp
browser.cpp
playbackcontrol.cpp
infodisplay.cpp
playlist.cpp
player.cpp
mediacenter.cpp
mediacenterstate.cpp
picturestate.cpp
videostate.cpp
musicstate.cpp
homestate.cpp
medialayout.cpp
private/mediahandler.cpp
private/sharedlayoutcomponentsmanager.cpp
)

kde4_add_library(mediacenterlibs SHARED ${mediacenterlibs_SRCS})

target_link_libraries(mediacenterlibs ${KDE4_PLASMA_LIBS} ${KDE4_SOLID_LIBS} ${KDE4_PHONON_LIBS} ${KDE4_NEPOMUK_LIBS} ${TAGLIB_LIBRARIES})
install(TARGETS mediacenterlibs ${INSTALL_TARGETS_DEFAULT_ARGS})

set (mediacenterlibs_HDRS
    mediacenter.h
    browser.h
    playbackcontrol.h
    infodisplay.h
    playlist.h
    player.h
    mediacenterstate.h
    picturestate.h
    videostate.h
    musicstate.h
    homestate.h
    medialayout.h
    gesturerecognizer.h
    playbackgesture.h
    browsergesture.h
)

install (FILES ${mediacenterlibs_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediacenter COMPONENT Devel)
