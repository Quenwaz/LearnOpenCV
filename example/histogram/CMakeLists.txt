# CMakeList.txt : CMake project for image_container, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


set(TARGET histogram)


set(${TARGET}_INCLUDES ${OpenCV_INCLUDES} ${CMAKE_SOURCE_DIR})
set(${TARGET}_LIBRARIES ${OpenCV_LIBRARIES})


compile_to_exe(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR})

