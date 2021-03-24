# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("LearningOpenCV")



list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

include(base)

find_package(OpenCV)

if (OpenCV_FOUND)
	message(STATUS "find OpenCV...")
else()
	message(FATAL_ERROR "OpenCV library not found")
endif()

# Include sub-projects.
add_subdirectory ("classification")
