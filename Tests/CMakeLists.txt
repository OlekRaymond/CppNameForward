cmake_minimum_required(VERSION 3.10)

include(GetGtest.cmake)

# Enable testing
enable_testing()

# Create test executable
add_executable(unit_tests
    test_main.cpp
    # Add your test source files here
    # example: test_myclass.cpp
)

# Link against gtest
target_link_libraries(unit_tests
    PRIVATE
    GTest::GTest
    GTest::Main
)

# Add tests to CTest
add_test(NAME unit_tests COMMAND unit_tests)

# Include directories
target_include_directories(unit_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# Set compiler flags
set_target_properties(unit_tests
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
