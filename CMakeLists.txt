# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project name and version
project(Mp4Reader VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add an option to enable PugiXML
option(ENABLE_PUGIXML "Enable PugiXML support" ON)

# Add an executable target, specifying all source files
add_executable(Mp4Reader src/main.cpp src/Mp4Reader.cpp)

# Include header files
target_include_directories(Mp4Reader PUBLIC "${PROJECT_SOURCE_DIR}/src")

# Optionally, you can specify compile options (e.g., warnings)
# target_compile_options(Mp4Reader PRIVATE -Wall -Wextra -Wpedantic)

# Conditionally fetch and include PugiXML
if(ENABLE_PUGIXML)
    include(FetchContent)

    FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml.git
        GIT_TAG v1.14
    )

    FetchContent_MakeAvailable(pugixml)

    add_compile_definitions(PUGIXML_ENABLED)
    # Link PugiXML to your project and include the source directory
    target_link_libraries(Mp4Reader PRIVATE pugixml)
    target_include_directories(Mp4Reader PRIVATE ${pugixml_SOURCE_DIR}/src)
    
endif()

# Detect Debug build and include CppUTest
if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    message(STATUS "Debug mode detected. Enabling AddressSanitizer...")

    # Check for the compiler
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Add AddressSanitizer flags for compilation and linking
        target_compile_options(Mp4Reader PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_libraries(Mp4Reader PRIVATE -fsanitize=address)

        # Optionally, enable leak detection settings
        set(ASAN_OPTIONS "detect_leaks=1")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    endif()

    message(STATUS "Enabling CppUTest...")

    # Use FetchContent to automatically download and set up CppUTest
    include(FetchContent)

    FetchContent_Declare(
        cpputest
        GIT_REPOSITORY https://github.com/cpputest/cpputest.git
        GIT_TAG v4.0  # You can specify a specific release, e.g., v4.0
    )

    # Fetch CppUTest
    FetchContent_MakeAvailable(cpputest)

    # Add test executable
    add_executable(Mp4ReaderTests
        src/Mp4Reader.cpp
        tests/main.cpp
        tests/test_Mp4Reader.cpp
    )

    add_test(NAME Mp4ReaderTestSuite COMMAND Mp4ReaderTests)

    # Enable testing
    enable_testing()

    # Link CppUTest to the test executable
    target_link_libraries(Mp4ReaderTests PRIVATE CppUTest)

    # Set include directories for the test executable
    target_include_directories(Mp4ReaderTests PRIVATE "${PROJECT_SOURCE_DIR}/src")
    
    if(ENABLE_PUGIXML)
        target_sources(Mp4ReaderTests PRIVATE tests/test_Mp4ReaderWithXML.cpp)
        target_link_libraries(Mp4ReaderTests PRIVATE pugixml)
        target_include_directories(Mp4ReaderTests PRIVATE ${pugixml_SOURCE_DIR}/src)
    endif()

    # Ensure tests are only built in Debug mode
    set_target_properties(Mp4ReaderTests PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

