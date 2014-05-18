enable_testing()
include(CTest)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(mockcpp REQUIRED)

include_directories(
    ${mockcpp_INCLUDE_DIRS}
)

function(pmc_test TESTNAME)
    add_executable(${TESTNAME} ${TESTNAME}.cpp)
    add_test(pmc-libs-${TESTNAME} ${TESTNAME})
    target_link_libraries(${TESTNAME}
        plasmamediacentertest
        Qt5::Test
        ${mockcpp_LIBRARIES}
    )
endfunction(pmc_test)

pmc_test(medialibrarytest)
pmc_test(singletonfactorytest)
pmc_test(lastfmimagefetchertest)
pmc_test(pmcmediatest)
pmc_test(mediatest)
pmc_test(mediacentertest)
pmc_test(itemcachetest)
pmc_test(pmcalbumtest)
pmc_test(pmcartisttest)
