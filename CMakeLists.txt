cmake_minimum_required(VERSION 3.10)
project(searchtoofast VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
    src/utils/tokenizer.cpp
    src/utils/json_helper.cpp
    src/application/window.cpp
    src/application/app.cpp
    src/application/config/config.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(SIGCMM REQUIRED sigc++-2.0)
pkg_check_modules(PCOV QUIET libcov)

find_package(Boost CONFIG REQUIRED COMPONENTS json)

# Main executable
add_executable(${PROJECT_NAME}
    ${CORE_SOURCES}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${GTKMM_INCLUDE_DIRS}
    ${GLIBMM_INCLUDE_DIRS}
    ${SIGCMM_INCLUDE_DIRS}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${GTKMM_CFLAGS_OTHER}
    ${GLIBMM_CFLAGS_OTHER}
    ${SIGCMM_CFLAGS_OTHER}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GTKMM_LIBRARIES}
    ${GLIBMM_LIBRARIES}
    ${SIGCMM_LIBRARIES}
    ${Boost_LIBRARIES}
)

# Code coverage: apply flags and optional libcov linking after target exists
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage -O0 -g)
        if(PCOV_FOUND)
            target_link_libraries(${PROJECT_NAME} PRIVATE ${PCOV_LIBRARIES})
        endif()
    else()
        message(WARNING "Code coverage is only supported with GCC and Clang.")
    endif()
endif()


# Optional: Test target
if(BUILD_TESTS)
    enable_testing()
    # Tests will be added here
endif()
