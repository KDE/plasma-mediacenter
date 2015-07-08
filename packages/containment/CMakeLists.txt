set(mediacentercontainment_SRCS
    mediacenter.cpp
)

add_library(plasma_containment_mediacenter MODULE ${mediacentercontainment_SRCS})

target_link_libraries(plasma_containment_mediacenter
    KF5::Plasma
    KF5::Service
)

install(TARGETS plasma_containment_mediacenter DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/applets)

kcoreaddons_desktop_to_json(plasma_containment_mediacenter package/metadata.desktop)

plasma_install_package(package org.kde.mediacentercontainment plasmoids containment)
