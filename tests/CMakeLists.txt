cmake_minimum_required(VERSION 3.8)

add_executable(test tests.cpp)
target_compile_features(test PRIVATE cxx_lambdas)
