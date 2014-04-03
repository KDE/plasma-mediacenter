enable_testing()
include(CTest)

find_package(QxOrm REQUIRED)
find_package(Boost REQUIRED COMPONENTS serialization)

include_directories(
   ${QxOrm_INCLUDE_DIRS}
)

add_definitions(-D_QX_SERIALIZE_BINARY_ENABLED -D_QX_SERIALIZE_XML_ENABLED -D_BUILDING_PMC)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

function(pmc_test TESTNAME)
    qt4_automoc(${TESTNAME}.cpp fakemediavalidator.cpp)
    add_executable(${TESTNAME} ${TESTNAME}.cpp fakemediavalidator.cpp)
    add_test(pmc-libs-${TESTNAME} ${TESTNAME})
    target_link_libraries(${TESTNAME} plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTSQL_LIBRARIES} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES} ${Boost_LIBRARIES} ${QxOrm_LIBRARIES})
endfunction(pmc_test)

pmc_test(medialibrarytest)
pmc_test(singletonfactorytest)
pmc_test(lastfmimagefetchertest)
pmc_test(pmcmediatest)
pmc_test(mediatest)
pmc_test(mediacentertest)
