cmake_minimum_required(VERSION 3.15...3.28)

project(CppNameForward VERSION 0.1.0 LANGUAGES CXX)

# Option requested in your prompt. Turn tests on with -DBUILD_TESTING=ON
option(BUILD_TESTING "Build tests" OFF)

# Header-only interface library that provides the include path for <Name/...>
add_library(Name_Forward INTERFACE)

target_include_directories(Name_Forward
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Request C++14 or higher for consumers
target_compile_features(Name_Forward INTERFACE cxx_std_14)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(Tests)
endif()

