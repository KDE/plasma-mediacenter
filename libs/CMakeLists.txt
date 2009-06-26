include_directories(${CMAKE_CURRENT_SOURCE_DIR}
)
set (mediacenterlibs_SRCS
browser.cpp
playbackcontrol.cpp
)

kde4_add_library(mediacenterlibs SHARED ${mediacenterlibs_SRCS})

target_link_libraries(mediacenterlibs ${KDE4_PLASMA_LIBS})
install(TARGETS mediacenterlibs ${INSTALL_TARGETS_DEFAULT_ARGS})

set (mediacenterlibs_HDRS
    mediacenter_export.h
    mediacenter.h
    browser.h
    playbackcontrol.h
)

install (FILES ${mediacenterlibs_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediacenter COMPONENT Devel)
