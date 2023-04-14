cmake_minimum_required(VERSION 3.14...3.22)

project(bnbicp_example LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.0.0
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(
  GITHUB_REPOSITORY pybind/pybind11
  VERSION 2.10.4
)

CPMAddPackage(NAME bnbicp SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

add_executable(bnb_simple_example ${CMAKE_CURRENT_SOURCE_DIR}/src/bnb_simple_example.cpp)
set_target_properties(bnb_simple_example PROPERTIES CXX_STANDARD 17)
target_link_libraries(bnb_simple_example PRIVATE Ceres::ceres pybind11::embed)
