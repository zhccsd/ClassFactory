cmake_minimum_required(VERSION 3.10)
project(TestClassFactory LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_RTTI "Enable Run-Time Type Information" ON)

add_executable(${PROJECT_NAME} classfactory.hpp test.cpp)

if(NOT ENABLE_RTTI)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/GR->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fno-rtti>
    )
endif()
