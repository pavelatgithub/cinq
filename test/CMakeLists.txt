set(TEST_SOURCES
  test.cpp
)
add_executable(unittest ${TEST_SOURCES})
add_test(NAME CINQ_Test COMMAND unittest)
