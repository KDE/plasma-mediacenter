install(FILES colors metadata.desktop DESTINATION ${DATA_INSTALL_DIR}/mediacentertheme/oxygen-mediacenter/)

FILE(GLOB widgets widgets/*.svgz)
install( FILES ${widgets} DESTINATION ${DATA_INSTALL_DIR}/mediacentertheme/oxygen-mediacenter/widgets/ )
FILE(GLOB dialogs dialogs/*.svgz)
install( FILES ${dialogs} DESTINATION ${DATA_INSTALL_DIR}/mediacentertheme/oxygen-mediacenter/dialogs/ )
FILE(GLOB icons icons/*.svgz)
install( FILES ${icons} DESTINATION ${DATA_INSTALL_DIR}/mediacentertheme/oxygen-mediacenter/icons/ )

# Install wallpaper globally, so it can be found from the config
#install(DIRECTORY wallpapers/HorosGreen DESTINATION ${WALLPAPER_INSTALL_DIR} PATTERN .svn EXCLUDE)


