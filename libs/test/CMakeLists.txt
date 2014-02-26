enable_testing()
include(CTest)

set (CMAKE_CXX_FLAGS "-D_QX_SERIALIZE_BINARY_ENABLED -D_QX_SERIALIZE_XML_ENABLED -D_BUILDING_PMC")

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

qt4_automoc(medialibrarytest.cpp)
add_executable(medialibrarytest medialibrarytest.cpp)
add_test(pmc-libs-medialibrarytest medialibrarytest)
target_link_libraries(medialibrarytest plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTSQL_LIBRARIES} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES} -lboost_serialization -lQxOrm)

qt4_automoc(singletonfactorytest.cpp)
add_executable(singletonfactorytest singletonfactorytest.cpp)
add_test(pmc-libs-singletonfactorytest singletonfactorytest)
target_link_libraries(singletonfactorytest plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTSQL_LIBRARIES} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES} -lboost_serialization -lQxOrm)
