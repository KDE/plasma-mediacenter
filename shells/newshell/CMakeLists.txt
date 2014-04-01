project (plasma-mediacenter)

include_directories(${KDE4_INCLUDES})

set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})

set(plasma-mediacenter_SRCS
    settings.cpp
    main.cpp
    mainwindow.cpp
    application.cpp
)

kde4_add_executable(plasma-mediacenter ${plasma-mediacenter_SRCS})
target_link_libraries(plasma-mediacenter
    plasmamediacenter
    ${KDE4_KDEUI_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_QTOPENGL_LIBRARIES}
    ${QT_QTDECLARATIVE_LIBRARIES}
    ${QT_MULTIMEDIAKIT_LIBRARIES}
    kdeclarative
)

install(TARGETS plasma-mediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.plasma.mediacenter)
install(FILES plasma-mediacenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

macro_display_feature_log()
