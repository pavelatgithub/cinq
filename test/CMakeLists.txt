set(TEST_SOURCES
    main.cpp
    join_test.cpp
    sum_test.cpp
    where_test.cpp
)
add_executable(unittest ${TEST_SOURCES})
add_test(NAME CINQ_Test COMMAND unittest)
