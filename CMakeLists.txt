cmake_minimum_required(VERSION 3.10)
project(MatrixTests)
set(CMAKE_CXX_STANDARD 17)
add_executable(MatrixTests source/tests.cpp)

# Dependency on ValueLib
add_subdirectory(gaba-linalg/)
target_link_libraries(MatrixTests PUBLIC LinAlgLib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(MatrixTests PUBLIC GTest::gtest_main)
