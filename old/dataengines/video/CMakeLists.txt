include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
)

set(video_engine_SRCS
video.cpp
)

kde4_add_plugin(plasma_engine_video ${video_engine_SRCS})
target_link_libraries(plasma_engine_video videoprovidercore ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS plasma_engine_video DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-video.desktop DESTINATION ${SERVICES_INSTALL_DIR})

set(videoprovider_core_SRCS
    videoprovider/videoprovider.cpp
)

kde4_add_library(videoprovidercore SHARED ${videoprovider_core_SRCS})
target_link_libraries(videoprovidercore ${KDE4_KDECORE_LIBS})
install(TARGETS videoprovidercore ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES videoprovider.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

set (videoprovider_core_HDRS
    videoprovider/videoprovider.h
)

install(FILES ${videoprovider_core_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/videoprovider COMPONENT Devel)
