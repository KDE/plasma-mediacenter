project(youtubeprovider)
include_directories(../../dataengines/video)

set(youtubeprovider_SRCS
    youtubeinterface.cpp
    youtubeprovider.cpp
)

kde4_add_plugin(videoprovider_youtube ${youtubeprovider_SRCS})
target_link_libraries(videoprovider_youtube videoprovidercore ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS videoprovider_youtube DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES videoprovider-youtube.desktop DESTINATION ${SERVICES_INSTALL_DIR}) 
