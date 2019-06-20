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
OPTION(COMPACT_IP OFF "Test libcr with more compact coroutines.")

if(RELEASE)
	message("libcr-test: RELEASE")
else()
	message("libcr-test: DEBUG")
endif()

if(COMPACT_IP)
	message("ENABLED: compact instruction pointers.")
endif()

set(LIBCR_RELEASE ${RELEASE} CACHE BOOL "libcr build mode" FORCE)
set(LIBCR_COMPACT_IP ${COMPACT_IP} CACHE BOOL "libcr compact instruction pointers" FORCE)

add_subdirectory(depend/libcr)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -g")
if(RELEASE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBCR_RELEASE -DNDEBUG -O3 -mtune=native")
endif()
if(COMPACT_IP)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLIBCR_COMPACT_IP")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/depend/libcr/include)

# Select all source files.
file(GLOB_RECURSE libcr_test_sources ./src/*.cpp)
file(GLOB_RECURSE libcr_test_headers ./src/*.hpp ./src/*.inl)

# Add them to the project.
add_executable(libcr_test ${libcr_test_sources} ${libcr_test_headers})

target_link_libraries(libcr_test libcr pthread)