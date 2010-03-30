project(wiimote)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )


set(wiimote_SRCS
    main.cpp
    wiimote.cpp
    wiimotewidget.cpp
)

kde4_add_ui_files(wiimote_SRCS wiimoteinterface.ui)
kde4_add_executable(wiimote-demo ${wiimote_SRCS})

target_link_libraries(wiimote-demo ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} cwiid)

install(TARGETS wiimote-demo ${INSTALL_TARGETS_DEFAULT_ARGS})
