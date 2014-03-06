enable_testing()
include(CTest)

add_definitions(-D_QX_SERIALIZE_BINARY_ENABLED -D_QX_SERIALIZE_XML_ENABLED -D_BUILDING_PMC)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

qt4_automoc(medialibrarytest.cpp fakemediavalidator.cpp)
add_executable(medialibrarytest medialibrarytest.cpp fakemediavalidator.cpp)
add_test(pmc-libs-medialibrarytest medialibrarytest)
target_link_libraries(medialibrarytest plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTSQL_LIBRARIES} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES} -lboost_serialization -lQxOrm)

qt4_automoc(singletonfactorytest.cpp fakemediavalidator.cpp)
add_executable(singletonfactorytest singletonfactorytest.cpp fakemediavalidator.cpp)
add_test(pmc-libs-singletonfactorytest singletonfactorytest)
target_link_libraries(singletonfactorytest plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTSQL_LIBRARIES} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES} -lboost_serialization -lQxOrm)

qt4_automoc(lastfmimagefetchertest.cpp fakemediavalidator.cpp)
add_executable(lastfmimagefetchertest lastfmimagefetchertest.cpp fakemediavalidator.cpp)
add_test(pmc-libs-lastfmimagefetchertest lastfmimagefetchertest)
target_link_libraries(lastfmimagefetchertest plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTSQL_LIBRARIES} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES} -lboost_serialization -lQxOrm)
