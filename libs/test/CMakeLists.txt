enable_testing()
include(CTest)
include(ECMAddTests)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(mockcpp REQUIRED)

include_directories(
    ${mockcpp_INCLUDE_DIRS}
)

set(pmc_test_SRCS
    medialibrarytest.cpp
    singletonfactorytest.cpp
    lastfmimagefetchertest.cpp
    pmcmediatest.cpp
    mediatest.cpp
    mediacentertest.cpp
    itemcachetest.cpp
    pmcalbumtest.cpp
    pmcartisttest.cpp
)

ecm_add_tests(
    ${pmc_test_SRCS}
    LINK_LIBRARIES plasmamediacentertest Qt5::Test ${mockcpp_LIBRARIES}
    NAME_PREFIX "pmctest-"
    )
