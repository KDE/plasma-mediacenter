project(pmc-app)

set(pmc-app_SRCS
    pmccorona.cpp
    pmcview.cpp
    main.cpp
)

add_executable(plasma-mediacenter ${pmc-app_SRCS})

target_link_libraries(
    plasma-mediacenter
    Qt5::Widgets
    Qt5::Quick
    Qt5::Qml
    KF5::PlasmaQuick
    KF5::Plasma
)

install(TARGETS plasma-mediacenter ${INSTALL_TARGETS_DEFAULT_ARGS})
