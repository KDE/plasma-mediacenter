project (plasma-mediacenter)
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include_directories(${KDE4_INCLUDES})

set(plasma-mediacenter_SRCS
main.cpp
mainwindow.cpp
)
kde4_add_ui_files(plasma-mediacenter_SRCS theme.ui backgrounddialog.ui)
kde4_add_executable(plasma-mediacenter ${plasma-mediacenter_SRCS})
target_link_libraries(plasma-mediacenter mediacenterlibs ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS plasma-mediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})
