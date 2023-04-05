cmake_minimum_required (VERSION 3.18)

set(IS_SUB_PROJECT 1)
set(RC_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

if (NOT DEFINED RC_LOCAL_REPO_PATH)
    if (${IS_SUB_PROJECT} EQUAL 1)
        set(RC_LOCAL_REPO_PATH "${CMAKE_SOURCE_DIR}/../../")
    else ()
        set(RC_LOCAL_REPO_PATH "${CMAKE_SOURCE_DIR}/../")
    endif ()

    message("Path to RC repo not defined. Using default: '${RC_LOCAL_REPO_PATH}'")
endif()

if (DEFINED RC_COMMON_CMAKE_FILE)
    include(${RC_COMMON_CMAKE_FILE})
else()
    include(${RC_LOCAL_REPO_PATH}/CMake/RC_Common.cmake)
endif()

set(TARGET File_Testing)
project(${TARGET})

add_executable(${TARGET} "main.cpp")
target_compile_features(${TARGET} PUBLIC cxx_std_20)

add_rc_dependency_to_target(${TARGET} File)
