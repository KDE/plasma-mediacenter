include_directories(../../)
set(mctest_SRCS
test.cpp
../../widgets/activeitemwidget.cpp
)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_mctest ${mctest_SRCS})
target_link_libraries(plasma_applet_mctest
                      ${KDE4_PLASMA_LIBS} mediacenterlibs)
 
install(TARGETS plasma_applet_mctest
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-mctest.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
