set(youtubebackend_SRCS
    diskcache.cpp
    networkaccess.cpp
    videodefinition.cpp
    networkaccess.cpp
    video.cpp
    youtubemodel.cpp
    youtubebackend.cpp
    videodetailsmodel.cpp
)

qt4_automoc(${youtubebackend_SRCS})

kde4_add_plugin(pmc_youtubebackend ${youtubebackend_SRCS})
target_link_libraries(pmc_youtubebackend
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_QTXML_LIBRARY}
)

install(TARGETS pmc_youtubebackend DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES youtubebackend.desktop DESTINATION ${SERVICES_INSTALL_DIR})
