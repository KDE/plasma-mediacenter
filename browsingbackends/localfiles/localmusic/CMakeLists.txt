set(localmusicbackend_SRCS
  localmusicmodel.cpp
  ../localfilesabstractbackend.cpp
  ../localfilesabstractmodel.cpp
  localmusicbackend.cpp
)

soprano_add_ontology(localmusicbackend_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig "NFO" "Vocabulary" "trig")
soprano_add_ontology(localmusicbackend_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig "NIE" "Vocabulary" "trig")

kde4_add_plugin(pmc_localmusic ${localmusicbackend_SRCS})

target_link_libraries(pmc_localmusic 
    plasmamediacenter
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KFILE_LIBS}
    ${NEPOMUK_LIBRARIES}
    ${NEPOMUK_QUERY_LIBRARIES}
    ${SOPRANO_LIBRARIES}
    ${TAGLIB_LIBRARIES}
)

install(TARGETS pmc_localmusic DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES localmusic.desktop DESTINATION ${SERVICES_INSTALL_DIR})
