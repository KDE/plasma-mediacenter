MACRO(INSTALL_JAVASCRIPT_ADDON dir addonPrefix)
       MESSAGE("macro says ${dir}" )
       configure_file(${dir}/metadata.desktop plasma-javascriptaddon-${addonPrefix}.desktop COPYONLY)
       install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plasma-javascriptaddon-${addonPrefix}.desktop DESTINATION ${SERVICES_INSTALL_DIR})
       install(FILES ${dir}/metadata.desktop DESTINATION ${DATA_INSTALL_DIR}/plasma/javascript-addons/${addonPrefix}/)

       install(FILES ${dir}/contents/code/main.js DESTINATION ${DATA_INSTALL_DIR}/plasma/javascript-addons/${addonPrefix}/contents/code/)
ENDMACRO(INSTALL_JAVASCRIPT_ADDON)

INSTALL_JAVASCRIPT_ADDON(flickr org.kde.plasma.dataengine.webmedia.flickr)

