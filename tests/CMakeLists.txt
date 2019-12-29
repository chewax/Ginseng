cmake_minimum_required(VERSION 3.8)

add_executable(libtest tests.cpp)
target_compile_features(libtest PRIVATE cxx_lambdas)
