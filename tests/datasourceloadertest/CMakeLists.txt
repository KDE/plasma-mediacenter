set (pmc_SRCS
    application.cpp
    )

set(datasourcesloadertest_SRCS
    datasourceloadertest.cpp
)

add_executable(datasourcesloadertest ${datasourcesloadertest_SRCS})
target_link_libraries(datasourcesloadertest
    plasmamediacenter
)
