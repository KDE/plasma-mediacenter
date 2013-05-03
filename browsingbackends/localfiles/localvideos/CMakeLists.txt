set(localvideosbackend_SRCS
  localvideosmodel.cpp
  ../localfilesabstractbackend.cpp
  ../localfilesabstractmodel.cpp
  localvideosbackend.cpp
  localvideothumbnailprovider.cpp
)


kde4_add_plugin(pmc_localvideos ${localvideosbackend_SRCS})
target_link_libraries(pmc_localvideos 
plasmamediacenter
${KDE4_KDECORE_LIBS}
${KDE4_KIO_LIBS}
${KDE4_KFILE_LIBS}
${QT_QTDECLARATIVE_LIBRARIES}
${KDE4_SOLID_LIBS}
)

install(TARGETS pmc_localvideos DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localvideos.desktop DESTINATION ${SERVICES_INSTALL_DIR})
