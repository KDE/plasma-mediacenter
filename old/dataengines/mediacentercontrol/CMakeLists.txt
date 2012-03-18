# A name for the project
project(plasma-mediacentercontrol)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# Adding source code here
set(mediacentercontrol_SRCS
    mediacentercontrol.cpp
    mediacontainer.cpp
    media.cpp
    mediaservice.cpp
    mediajob.cpp
)
 
# Make sure that all files get to the right place
kde4_add_plugin(plasma_engine_mediacentercontrol ${mediacentercontrol_SRCS})
target_link_libraries(plasma_engine_mediacentercontrol
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS})
 
install(TARGETS plasma_engine_mediacentercontrol
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-engine-mediacentercontrol.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES mediacentercontrol.operations DESTINATION ${DATA_INSTALL_DIR}/plasma/services)