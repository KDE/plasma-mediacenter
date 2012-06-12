set(localpicturesbackend_SRCS
    localpicturesmodel.cpp
    ../localfilesabstractbackend.cpp
    ../localfilesabstractmodel.cpp
    localpicturesbackend.cpp
)
soprano_add_ontology(localpicturesbackend_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig "NFO" "Vocabulary" "trig")
soprano_add_ontology(localpicturesbackend_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig "NIE" "Vocabulary" "trig")

kde4_add_plugin(pmc_localpictures ${localpicturesbackend_SRCS})
target_link_libraries(pmc_localpictures
plasmamediacenter
${KDE4_KDECORE_LIBS}
${KDE4_KIO_LIBS}
${KDE4_KFILE_LIBS}
${NEPOMUK_LIBRARIES}
${NEPOMUK_QUERY_LIBRARIES}
${SOPRANO_LIBRARIES}
)

install(TARGETS pmc_localpictures DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localpictures.desktop DESTINATION ${SERVICES_INSTALL_DIR})
