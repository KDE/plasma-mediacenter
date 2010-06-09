
set (mediabrowserlibs_SRCS
    abstractbrowsingbackend.cpp
)

kde4_add_library(mediabrowserlibs SHARED ${mediabrowserlibs_SRCS})

target_link_libraries(mediabrowserlibs ${KDE4_KDECORE_LIBS})
install(TARGETS mediabrowserlibs ${INSTALL_TARGETS_DEFAULT_ARGS})

set (mediabrowserlibs_HDRS
    abstractbrowsingbackend.h
    mediabrowser_export.h
)

install(FILES ${mediabrowserlibs_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/mediabrowserlibs COMPONENT Devel)
install(FILES pmc_browsingbackend.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
