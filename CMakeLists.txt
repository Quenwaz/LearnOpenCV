# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("LearningOpenCV")


list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)
set(CMAKE_BUILD_TYPE Debug)
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

include(base)

find_package(OpenCV)

if (OpenCV_FOUND)
	message(STATUS "find OpenCV...")
else()
	message(FATAL_ERROR "OpenCV library not found")
endif()

set(CMAKE_CREATE_CONSOLE_EXE "")
set(CMAKE_CREATE_WIN32_EXE  "")


# Include sub-projects.
add_subdirectory ("classification")
add_subdirectory ("image_container")
add_subdirectory ("images_stitching")
add_subdirectory ("histogram")
add_subdirectory ("erase_specify_color")
add_subdirectory ("camera_to_video")
add_subdirectory ("split_color")
add_subdirectory ("utils")
add_subdirectory ("motion_correction")
add_subdirectory ("live_ocr")

PRINT_ALL_VARIABLE()
