cmake_minimum_required(VERSION 3.10)
project(searchtoofast VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable code coverage" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(${PROJECT_SOURCE_DIR}/include)

# Core components
set(CORE_SOURCES
    src/core/indexer/file_indexer.cpp
    src/core/indexer/directory_indexer.cpp
    src/core/file_detection/file_detector.cpp
    src/core/search/search_engine.cpp
    src/searchtoofast.cpp
)

# Main executable
add_executable(${PROJECT_NAME}
    ${CORE_SOURCES}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optional: Test target
if(BUILD_TESTS)
    enable_testing()
    # Tests will be added here
endif()
