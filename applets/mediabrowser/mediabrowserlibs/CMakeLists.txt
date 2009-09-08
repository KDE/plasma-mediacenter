
set (mediabrowserlibs_SRCS
    modelpackage.cpp
)

kde4_add_library(mediabrowserlibs SHARED ${mediabrowserlibs_SRCS})

target_link_libraries(mediabrowserlibs ${KDE4_KDECORE_LIBS})
install(TARGETS mediabrowserlibs ${INSTALL_TARGETS_DEFAULT_ARGS})

set (mediabrowserlibs_HDRS
    modelpackage.h
    mediabrowser_export.h
)

install(FILES ${mediabrowserlibs_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediabrowserlibs COMPONENT Devel)
