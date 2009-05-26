
set(youtube_engine_SRCS
youtube.cpp
youtubeinterface.cpp
)

kde4_add_plugin(plasma_engine_youtube ${youtube_engine_SRCS})
target_link_libraries(plasma_engine_youtube ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY})

install(TARGETS plasma_engine_youtube DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-youtube.desktop DESTINATION ${SERVICES_INSTALL_DIR})
