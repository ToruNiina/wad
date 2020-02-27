cmake_minimum_required(VERSION 3.13)
enable_testing()
project(wad)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD          11)
set(CMAKE_CXX_EXTENSIONS       OFF)

option(WAD_TEST_COVERAGE "collecting test coverage" OFF)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extlib")
    execute_process(COMMAND mkdir extlib WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extlib/catch.hpp")
    file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.11.1/catch.hpp
         ${PROJECT_SOURCE_DIR}/extlib/catch.hpp
         STATUS WAD_DOWNLOAD_CATCH_STATUS)

     list(GET WAD_DOWNLOAD_CATCH_STATUS 0 WAD_DOWNLOAD_CATCH_RESULT)
     if(NOT ${WAD_DOWNLOAD_CATCH_RESULT} EQUAL 0)
         list(GET WAD_DOWNLOAD_CATCH_STATUS 1 WAD_DOWNLOAD_CATCH_ERROR)
         message(FATAL_ERROR "failed to download Catch2 library."
             ${WAD_DOWNLOAD_CATCH_ERROR})
     endif()
endif()

include_directories("${PROJECT_SOURCE_DIR}/extlib")
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(tests)
