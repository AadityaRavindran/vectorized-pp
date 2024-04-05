cmake_minimum_required(VERSION 3.8)

find_package(GTest REQUIRED)
add_executable(vecpp_test
  test_parser.cc
  test_planner.cc
)
target_link_libraries(vecpp_test
  PUBLIC
  vecpp
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(vecpp_test)
