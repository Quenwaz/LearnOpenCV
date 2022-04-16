# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("LearningOpenCV")


list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
set(CMAKE_BUILD_TYPE Debug)
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

include(base)
include(utils)

find_package(OpenCV)

if (OpenCV_FOUND)
	message(STATUS "find OpenCV...")
else()
	message(FATAL_ERROR "OpenCV library not found")
endif()

set(CMAKE_CREATE_CONSOLE_EXE "")
set(CMAKE_CREATE_WIN32_EXE  "")

# Include sub-projects.
subdirlist(subdirs ${CMAKE_CURRENT_SOURCE_DIR}/example)
foreach(subdir ${subdirs})
	if (NOT subdir STREQUAL common)
		add_subdirectory ("example/${subdir}")
	endif()
endforeach(subdir ${subdirs})


# PRINT_ALL_VARIABLE()
