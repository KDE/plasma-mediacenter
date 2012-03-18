project (plasma-mediacenter)
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include_directories(${KDE4_INCLUDES})

set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})

set(plasma-mediacenter_SRCS
    main.cpp
    mainwindow.cpp
)

kde4_add_executable(plasma-mediacenter ${plasma-mediacenter_SRCS})
target_link_libraries(plasma-mediacenter
    mediacenterlibs
    ${KDE4_KDEUI_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_LIBRARIES}
    ${QT_QTDECLARATIVE_LIBRARIES}
    kdeclarative
)

install(TARGETS plasma-mediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.plasma.mediacenter)
install(FILES plasma-mediacenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
