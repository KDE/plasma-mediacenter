set (pmc_SRCS
    application.cpp
    )

set(mediasourcesloadertest_SRCS
    mediasourceloadertest.cpp
)

add_executable(mediasourcesloadertest ${mediasourcesloadertest_SRCS})
target_link_libraries(mediasourcesloadertest
    plasmamediacenter
)
