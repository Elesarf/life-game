# Check minimum required CMake version
cmake_minimum_required(VERSION 3.7)

# Set project name (package name, source archive name)
project("life")

set(SRC_DIR ${CMAKE_SOURCE_DIR})
message("Sources dir: ${SRC_DIR}")
message("Sources dir: ${CMAKE_BINARY_DIR}")

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "cmake in-source build are not allowed - please call cmake from build directory")
endif()

# Set available configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Profile")

# Please select configuration using "cmake -DCMAKE_BUILD_TYPE=Debug" command
message("Configuration selected using -DCMAKE_BUILD_TYPE: ")

# Set configuration by default
if(NOT CMAKE_BUILD_TYPE)
    message("CMAKE_BUILD_TYPE not set - defaulting to Debug build")
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SOURCES
    src/main.cpp
    )

set(HEADERS
    )

set(OTHER
    )

set(ADDITIONAL_INCLUDE_DIRS
    )

# Search for parent directories of listed headers
set(INCLUDE_DIRS "")

foreach(_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# GCC Warnings
set(GCC_WARNINGS -Wall -Wextra -Werror)

# Additional libraries
set(LINKER_ADDITIONAL_LIBRARIES "")
set(LINKER_ADDITIONAL_FLAGS "")

# for additional information
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_options(-std=c++14 -g -O0 ${GCC_WARNINGS})
    add_definitions()
    set(LINKER_ADDITIONAL_LIBRARIES)
    set(LINKER_ADDITIONAL_FLAGS -g)
elseif(CMAKE_BUILD_TYPE STREQUAL Profile)
    add_compile_options(-std=c++14 ${GCC_WARNINGS})
    add_definitions()
    set(LINKER_ADDITIONAL_LIBRARIES "")
    set(LINKER_ADDITIONAL_FLAGS "")
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    add_compile_options(-std=c++14 -O2 ${GCC_WARNINGS})
    add_definitions()
    set(LINKER_ADDITIONAL_LIBRARIES)
    set(LINKER_ADDITIONAL_FLAGS "")
else()
    message(FATAL_ERROR "Unknown configuration selected: ${CMAKE_BUILD_TYPE}")
endif()

include_directories(${INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${ADDITIONAL_INCLUDE_DIRS})
message("INCLUDE_DIRS: ${INCLUDE_DIRS}")
message(" ")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(" ")
message("ADDITIONAL_INCLUDE_DIRS: ${ADDITIONAL_INCLUDE_DIRS}")
message(" ")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Provide executable name
set(TARGET life)

# Setup executable name and sources
# Note: ${HEADERS} list is added so AUTOMOC will run on qt headers (AUTOMOC fix)
add_executable(${TARGET} ${SOURCES} ${HEADERS} ${OTHER})

# Add linker libraries
target_link_libraries(
    ${TARGET}
    ${LINKER_ADDITIONAL_LIBRARIES}
    ${LINKER_ADDITIONAL_FLAGS}
)

target_include_directories(${TARGET} PRIVATE ./src)
target_include_directories(${TARGET} PUBLIC ./include)
