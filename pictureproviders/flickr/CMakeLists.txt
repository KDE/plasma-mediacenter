project(flickrprovider)
include_directories(../../dataengines/picture)

set(flickrprovider_SRCS
flickrprovider.cpp
flickrinterface.cpp
)

kde4_add_plugin(pictureprovider_flickr ${flickrprovider_SRCS})
target_link_libraries(pictureprovider_flickr pictureprovidercore ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS pictureprovider_flickr DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pictureprovider-flickr.desktop DESTINATION ${SERVICES_INSTALL_DIR})
