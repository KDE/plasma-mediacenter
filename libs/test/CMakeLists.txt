enable_testing()
include(CTest)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

function(pmc_test TESTNAME)
    add_executable(${TESTNAME} ${TESTNAME}.cpp fakemediavalidator.cpp)
    add_test(pmc-libs-${TESTNAME} ${TESTNAME})
    target_link_libraries(${TESTNAME}
        plasmamediacentertest
        Qt5::Test
    )
endfunction(pmc_test)

pmc_test(medialibrarytest)
pmc_test(singletonfactorytest)
pmc_test(lastfmimagefetchertest)
pmc_test(pmcmediatest)
pmc_test(mediatest)
pmc_test(mediacentertest)
