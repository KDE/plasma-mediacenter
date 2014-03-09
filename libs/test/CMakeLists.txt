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

add_executable(medialibrarytest medialibrarytest.cpp fakemediavalidator.cpp)
add_test(pmc-libs-medialibrarytest medialibrarytest)
target_link_libraries(medialibrarytest plasmamediacenter Qt5::Test Qt5::Sql)

add_executable(singletonfactorytest singletonfactorytest.cpp fakemediavalidator.cpp)
add_test(pmc-libs-singletonfactorytest singletonfactorytest)
target_link_libraries(singletonfactorytest plasmamediacenter Qt5::Test Qt5::Sql)

add_executable(lastfmimagefetchertest lastfmimagefetchertest.cpp fakemediavalidator.cpp)
add_test(pmc-libs-lastfmimagefetchertest lastfmimagefetchertest)
target_link_libraries(lastfmimagefetchertest plasmamediacenter Qt5::Test Qt5::Sql)
