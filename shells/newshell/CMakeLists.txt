project (plasma-mediacenter)
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (FindPkgConfig)
include_directories(${KDE4_INCLUDES})

set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})

pkg_check_modules(QT_MULTIMEDIAKIT QtMultimediaKit)
macro_log_feature(QT_MULTIMEDIAKIT_FOUND "QtMultimediaKit" "Delivers an easy to use interface to multimedia functions" "http://doc.qt.nokia.com/qtmobility-1.1-tp/qtmultimediakit.html" FALSE "" "Needed for better video playback performance")
if (QT_MULTIMEDIAKIT_FOUND)
    add_definitions(-DQT_MULTIMEDIA_KIT_FOUND)
    include_directories(${QT_MULTIMEDIAKIT_INCLUDE_DIRS})
    link_directories(${QT_MULTIMEDIAKIT_LIBRARY_DIRS})
endif (QT_MULTIMEDIAKIT_FOUND)

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
    ${QT_MULTIMEDIAKIT_LIBRARIES}
    kdeclarative
)

install(TARGETS plasma-mediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY package/ DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.plasma.mediacenter)
install(FILES plasma-mediacenter.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

macro_display_feature_log()
