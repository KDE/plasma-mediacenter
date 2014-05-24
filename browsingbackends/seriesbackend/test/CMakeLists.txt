enable_testing()
include(CTest)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

find_package(mockcpp REQUIRED)

include_directories(
    ${mockcpp_INCLUDE_DIRS}
)

function(pmc_test TESTNAME)
    kde4_add_executable(${TESTNAME} ${TESTNAME}.cpp ../cluster.cpp)
    add_test(pmc-libs-${TESTNAME} ${TESTNAME})
    target_link_libraries(${TESTNAME}
    plasmamediacentertest ${KDE4_KDECORE_LIBRARY} ${KDE4_KIO_LIBS}
    ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES} ${QT_QTNETWORK_LIBRARIES} ${QT_QTXML_LIBRARIES}  ${QT_QTDECLARATIVE_LIBRARIES}
    ${TAGLIB_LIBRARIES} ${mockcpp_LIBRARIES}
    )
endfunction(pmc_test)

pmc_test(clustertest)
