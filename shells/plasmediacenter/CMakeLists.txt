project (plasmediacenter)
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include_directories(${KDE4_INCLUDES})

set(plasmediacenter_SRCS
main.cpp
mainwindow.cpp
)
kde4_add_ui_files(plasmediacenter_SRCS theme.ui)
kde4_add_executable(plasmediacenter ${plasmediacenter_SRCS})
target_link_libraries(plasmediacenter ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS})

install(TARGETS plasmediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})
