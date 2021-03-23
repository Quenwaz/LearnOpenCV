

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCHITECTURE x64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCHITECTURE x86)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
message(STATUS ARCHITECTURE=${ARCHITECTURE})


if (CMAKE_BUILD_TYPE STREQUAL Debug)
	set(IsDebug true)
else(CMAKE_BUILD_TYPE STREQUAL Debug)
	set(IsDebug false)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)
message(STATUS CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
message(STATUS IsDebug=${IsDebug})





macro(compile_to_exe TARGET_NAME SRC_PATH)
	message(STATUS "--------------begin compile ${TARGET_NAME}----------------------")
	file(GLOB_RECURSE ${TARGET_NAME}_SRC "${SRC_PATH}/*.cpp" "${SRC_PATH}/*.cc")
	add_executable(${TARGET_NAME} ${${TARGET_NAME}_SRC})
	message(STATUS ${TARGET_NAME}_SRC=${${TARGET_NAME}_SRC})

	target_include_directories(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_INCLUDES})
	message(STATUS ${TARGET_NAME}_INCLUDES=${${TARGET_NAME}_INCLUDES})

	target_link_directories(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_LIBRARY_DIR})
	message(STATUS ${TARGET_NAME}_LIBRARY_DIR=${${TARGET_NAME}_LIBRARY_DIR})

	target_link_libraries(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_LIBRARIES})
	message(STATUS ${TARGET_NAME}_LIBRARIES=${${TARGET_NAME}_LIBRARIES})

	target_compile_options(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_C_OPTIONS})
	message(STATUS ${TARGET_NAME}_C_OPTIONS=${${TARGET_NAME}_C_OPTIONS})

	target_link_options(${TARGET_NAME} PRIVATE ${${TARGET_NAME}_L_OPTIONS})
	message(STATUS ${TARGET_NAME}_L_OPTIONS=${${TARGET_NAME}_L_OPTIONS})
	

	
endmacro(compile_to_exe)