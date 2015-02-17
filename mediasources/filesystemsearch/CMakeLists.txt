set(filesystemsearch_SRCS
    filesystemsearchmediasource.cpp
)

add_library(pmc_filesystemsearch MODULE ${filesystemsearch_SRCS})
set_target_properties(pmc_filesystemsearch PROPERTIES PREFIX "")

target_link_libraries(pmc_filesystemsearch
    plasmamediacenter
)

kcoreaddons_desktop_to_json(pmc_filesystemsearch filesystemsearch.desktop)

install(TARGETS pmc_filesystemsearch DESTINATION ${PLUGIN_INSTALL_DIR}/plasma/mediacenter/mediasources)
