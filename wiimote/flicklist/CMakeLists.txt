# Project Needs a name ofcourse
#project(plasma-flicklist)
 
# Find the required Libaries
#find_package(KDE4 REQUIRED)
#include(KDE4Defaults)
 
#add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})


include_directories(
#   ${CMAKE_SOURCE_DIR}
#   ${CMAKE_BINARY_DIR}
#   ${KDE4_INCLUDES}
    ../
   )
 
# We add our source code here
set(flicklist_SRCS  flicklist.cpp
                    listwidget.cpp
                    ../wiimote.cpp
)
 
kde4_add_plugin(plasma_applet_flicklist ${flicklist_SRCS})

target_link_libraries(plasma_applet_flicklist 
                        ${KDE4_PLASMA_LIBS}
                        ${KDE4_KDEUI_LIBS}
                        ${KDE4_KIO_LIBS}
                        cwiid
                        bluetooth
)
 
install(TARGETS plasma_applet_flicklist
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-flicklist.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
