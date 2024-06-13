cmake_minimum_required(VERSION 3.28)
project(Satellite_simulator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include_directories(${CMAKE_SOURCE_DIR}/include/SDL)

add_executable(Satellite_sim)

target_sources(Satellite_sim PUBLIC
	FILE_SET all_my_modules TYPE CXX_MODULES
	FILES
		${CMAKE_SOURCE_DIR}/src/Window_module/window_interface.cppm
		${CMAKE_SOURCE_DIR}/src/Window_module/window.cppm
		${CMAKE_SOURCE_DIR}/src/Compute_module/compute.cppm
		${CMAKE_SOURCE_DIR}/src/Main_module/main.cppm
)

target_sources(Satellite_sim PUBLIC
	${CMAKE_SOURCE_DIR}/src/source.cpp
)

target_link_libraries(Satellite_sim
	${CMAKE_SOURCE_DIR}/include/lib/SDL2.lib
	${CMAKE_SOURCE_DIR}/include/lib/SDL2_image.lib
	${CMAKE_SOURCE_DIR}/include/lib/SDL2main.lib)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Satellite_sim)

add_custom_command(TARGET Satellite_sim
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/include/SDL2.dll
		${CMAKE_SOURCE_DIR}/bin/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/include/SDL2_image.dll 
		${CMAKE_SOURCE_DIR}/bin/Debug/
)