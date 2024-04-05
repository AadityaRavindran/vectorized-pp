cmake_minimum_required(VERSION 3.8)

project(
  vectorized_pp
  VERSION 0.0.1
  DESCRIPTION "A planner which acts on a vector map of polygon obstacles"
  LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ENABLE_TESTING ON)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(
 include/
 ${Boost_INCLUDE_DIRS}
)

add_library(vecpp
  src/parser.cpp
  src/planner.cpp
)
target_link_libraries(vecpp PRIVATE
  ${Boost_LIBRARIES}
  nlohmann_json::nlohmann_json
)

if (ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
