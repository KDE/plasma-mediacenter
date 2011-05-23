# A name for the project
project(plasma-datasource)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(datasource_SRCS
    datasource.cpp
)
#qt4_automoc(${datasource_SRCS})
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_engine_singleDataSource ${datasource_SRCS})
target_link_libraries(plasma_engine_singleDataSource
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS})
 
install(TARGETS plasma_engine_singleDataSource
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-engine-datasource.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})