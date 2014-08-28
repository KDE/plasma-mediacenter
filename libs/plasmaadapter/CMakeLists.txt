set (plasmaadapter_SRCS
    plasmaadapter.cpp
)

kde4_add_library(plasmaadapter SHARED
    ${plasmaadapter_SRCS}
)

target_link_libraries(plasmaadapter
    ${KDE4_PLASMA_LIBS}
)

set_target_properties(plasmaadapter PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS plasmaadapter ${INSTALL_TARGETS_DEFAULT_ARGS})
