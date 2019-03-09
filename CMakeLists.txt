cmake_minimum_required(VERSION 2.6)
project(libcr_test)

# Ensure all dependencies are existent.
execute_process(
	COMMAND git submodule init
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
execute_process(
	COMMAND git submodule update
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

OPTION(RELEASE OFF "Test libcr's release build.")

if(RELEASE)
	message("libcr-test: RELEASE")
else()
	message("libcr-test: DEBUG")
endif()

set(LIBCR_RELEASE ${RELEASE} CACHE BOOL "libcr build mode" FORCE)

add_subdirectory(depend/libcr)
add_subdirectory(depend/timer)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -g")
if(RELEASE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBCR_RELEASE -DNDEBUG")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/depend/libcr/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/depend/timer/include)

# Select all source files.
file(GLOB_RECURSE libcr_test_sources ./src/*.cpp)
file(GLOB_RECURSE libcr_test_headers ./src/*.hpp ./src/*.inl)

# Add them to the project.
add_executable(libcr_test ${libcr_test_sources} ${libcr_test_headers})

target_link_libraries(libcr_test libcr pthread)