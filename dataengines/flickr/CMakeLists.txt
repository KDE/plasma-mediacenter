set(flickr_engine_SRCS
flickr.cpp
flickrinterface.cpp
)

kde4_add_plugin(plasma_engine_flickr ${flickr_engine_SRCS})
target_link_libraries(plasma_engine_flickr ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY})

install(TARGETS plasma_engine_flickr DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-engine-flickr.desktop DESTINATION ${SERVICES_INSTALL_DIR})

