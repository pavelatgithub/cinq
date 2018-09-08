set(TEST_SOURCES
    main.cpp
    join_test.cpp
    sum_test.cpp
    where_test.cpp
)
add_executable(unittest ${TEST_SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/3rdparty/catch2")

include(CTest)
include(Catch)
catch_discover_tests(unittest)
