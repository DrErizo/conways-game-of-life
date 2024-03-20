find_package(SDL2 REQUIRED)
cmake_minimum_required(VERSION 3.14.15926)
project(conways-game-of-life)
set(CMAKE_C_STANDARD 99)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_compile_options(-Os)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
if(WIN32 AND CMAKE_SZIEOF_VOID_P EQUAL 4)
	message("Building for 32 bit Windows")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:IA32")
elseif()
	target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
