enable_testing()
include(CTest)

SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTTEST TRUE)

qt4_automoc(medialibrarytest.cpp)
add_executable(medialibrarytest medialibrarytest.cpp)
add_test(pmc-libs-medialibrarytest medialibrarytest)
target_link_libraries(medialibrarytest plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES})

qt4_automoc(singletonfactorytest.cpp)
add_executable(singletonfactorytest singletonfactorytest.cpp)
add_test(pmc-libs-singletonfactorytest singletonfactorytest)
target_link_libraries(singletonfactorytest plasmamediacenter ${KDE4_KDECORE_LIBRARY} ${QT_QTTEST_LIBRARIES} ${QT_QTGUI_LIBRARIES})
