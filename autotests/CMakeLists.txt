enable_testing()
include(CTest)
include(ECMAddTests)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

kde_enable_exceptions()

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(pmc_test_SRCS
    medialibrarytest.cpp
    singletonfactorytest.cpp
    pmcmediatest.cpp
    mediatest.cpp
    mediacentertest.cpp
    itemcachetest.cpp
    pmcalbumtest.cpp
    pmcartisttest.cpp
    runtimedatatest.cpp
)

ecm_add_tests(
    ${pmc_test_SRCS}
    LINK_LIBRARIES plasmamediacentertest Qt5::Test
    NAME_PREFIX "pmctest-"
    )
