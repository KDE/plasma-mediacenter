find_package(Taglib)
include_directories (${TAGLIB_INCLUDES})

set(localmusicbackend_SRCS
  localmusicmodel.cpp
  ../localfilesabstractbackend.cpp
  ../localfilesabstractmodel.cpp
  ../localplacesmodel.cpp
  localmusicbackend.cpp
)

kde4_add_plugin(pmc_localmusic ${localmusicbackend_SRCS})

target_link_libraries(pmc_localmusic 
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KFILE_LIBS}
    ${TAGLIB_LIBRARIES}
    ${KDE4_SOLID_LIBS}
    ${QT_QTDECLARATIVE_LIBRARIES}
)

install(TARGETS pmc_localmusic DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localmusic.desktop DESTINATION ${SERVICES_INSTALL_DIR})
