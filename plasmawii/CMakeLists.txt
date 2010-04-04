project(plasmawii)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )


set(plasmawii_SRCS
    main.cpp
)

kde4_add_executable(plasmawii ${plasmawii_SRCS})

target_link_libraries(plasmawii ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} cwiid)

<<<<<<< Updated upstream:MediaCenterComponents/plasmawii/CMakeLists.txt
install(TARGETS plasmawii ${INSTALL_TARGETS_DEFAULT_ARGS})
=======
install(TARGETS wiimote-demo ${INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(flicklist)
>>>>>>> Stashed changes:MediaCenterComponents/plasmawii/CMakeLists.txt
