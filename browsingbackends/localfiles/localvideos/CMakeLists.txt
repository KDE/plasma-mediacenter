set(localvideosbackend_SRCS
  localvideosmodel.cpp
  ../localfilesabstractbackend.cpp
  ../localfilesabstractmodel.cpp
  localvideosbackend.cpp
)

soprano_add_ontology(localvideosbackend_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig "NFO" "Vocabulary" "trig")
soprano_add_ontology(localvideosbackend_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig "NIE" "Vocabulary" "trig")

kde4_add_ui_files(localvideosbackend_SRCS ../configbase.ui)

kde4_add_plugin(pmc_localvideos ${localvideosbackend_SRCS})
target_link_libraries(pmc_localvideos 
mediacenterlibs
${KDE4_KDECORE_LIBS}
${KDE4_KIO_LIBS}
${KDE4_KFILE_LIBS}
${NEPOMUK_LIBRARIES}
${NEPOMUK_QUERY_LIBRARIES}
${SOPRANO_LIBRARIES}
)

install(TARGETS pmc_localvideos DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localvideos.desktop DESTINATION ${SERVICES_INSTALL_DIR})
