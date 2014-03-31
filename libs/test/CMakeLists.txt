enable_testing()
include(CTest)

add_definitions(-D_QX_SERIALIZE_BINARY_ENABLED -D_QX_SERIALIZE_XML_ENABLED -D_BUILDING_PMC)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)


function(pmc_test TESTNAME)
    add_executable(${TESTNAME} ${TESTNAME}.cpp fakemediavalidator.cpp)
    add_test(pmc-libs-${TESTNAME} ${TESTNAME})
    target_link_libraries(${TESTNAME} plasmamediacenter Qt5::Test Qt5::Sql)
endfunction(pmc_test)

pmc_test(medialibrarytest)
pmc_test(singletonfactorytest)
pmc_test(lastfmimagefetchertest)
pmc_test(pmcmediatest)
pmc_test(mediatest)
