cmake_minimum_required(VERSION 3.10)

project(astrum VERSION 0.1.0)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -DDEBUG -g ")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fno-omit-frame-pointer ")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address ")

string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O2 ")

string(APPEND CMAKE_CXX_FLAGS " -fsigned-char -Wall -Wextra -Werror")

option(DEFAULT_FONT "include a default font in the package" ON)
if(NOT DEFAULT_FONT)
	add_compile_definitions(NO_DEFAULT_FONT)
endif(NOT DEFAULT_FONT)

if(EMSCRIPTEN)
	add_compile_options(-sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2
		-sUSE_SDL_GFX=2)
	add_link_options(-sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2
		-sUSE_SDL_GFX=2 -lidbfs.js -sEXPORTED_FUNCTIONS=_log_fs_error,_main
		-sEXPORTED_RUNTIME_METHODS=ccall)
	set(CMAKE_EXECUTABLE_SUFFIX .html)
else(EMSCRIPTEN)
	find_package(SDL2 REQUIRED)
	find_package(SDL2TTF REQUIRED)
	find_package(SDL2IMAGE REQUIRED)
	find_package(SDL2GFX REQUIRED)
	find_package(OpenGL REQUIRED)
endif(EMSCRIPTEN)

add_library(astrum)
target_sources(astrum PRIVATE src/astrum.cpp src/font.cpp src/graphics.cpp
	src/keyboard.cpp src/math.cpp src/mouse.cpp src/window.cpp src/util.cpp
	src/image.cpp src/timer.cpp src/log.cpp src/filesystem.cpp src/audio.cpp
	src/sound.cpp src/system.cpp)
target_include_directories(astrum PUBLIC include)

if(ipo_supported AND CMAKE_BUILD_TYPE STREQUAL "Release")
	set_property(GLOBAL PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	message(STATUS "Interprocedural optimization not supported")
endif(ipo_supported AND CMAKE_BUILD_TYPE STREQUAL "Release")

if(NOT EMSCRIPTEN)
	target_include_directories(astrum PUBLIC ${SDL2_INCLUDE_DIRS})
	target_include_directories(astrum PUBLIC ${OPENGL_INCLUDE_DIRS})
	target_include_directories(astrum PUBLIC ${SDL2TTF_INCLUDE_DIR})
	target_include_directories(astrum PUBLIC ${SDL2IMAGE_INCLUDE_DIR})
	target_include_directories(astrum PUBLIC ${SDL2GFX_INCLUDE_DIR})
	target_link_libraries(astrum PUBLIC ${SDL2_LIBRARY})
	target_link_libraries(astrum PUBLIC ${OPENGL_gl_LIBRARY})
	target_link_libraries(astrum PUBLIC ${SDL2TTF_LIBRARY})
	target_link_libraries(astrum PUBLIC ${SDL2IMAGE_LIBRARY})
	target_link_libraries(astrum PUBLIC ${SDL2GFX_LIBRARY})
endif(NOT EMSCRIPTEN)

add_executable(astrumDemo examples/demo.cpp)
add_dependencies(astrumDemo astrum)
target_include_directories(astrumDemo PRIVATE astrum)
target_link_libraries(astrumDemo astrum)

add_executable(astrumPong examples/pong.cpp)
add_dependencies(astrumPong astrum)
target_include_directories(astrumPong PRIVATE astrum)
target_link_libraries(astrumPong astrum)

add_executable(astrumCircles examples/circles.cpp)
add_dependencies(astrumCircles astrum)
target_include_directories(astrumCircles PRIVATE astrum)
target_link_libraries(astrumCircles astrum)

add_executable(astrumSudoku examples/sudoku.cpp)
add_dependencies(astrumSudoku astrum)
target_include_directories(astrumSudoku PRIVATE astrum)
target_link_libraries(astrumSudoku astrum)

add_executable(astrumGameOfLife examples/life.cpp)
add_dependencies(astrumGameOfLife astrum)
target_include_directories(astrumGameOfLife PRIVATE astrum)
target_link_libraries(astrumGameOfLife astrum)

add_executable(astrumSnake examples/snake.cpp)
add_dependencies(astrumSnake astrum)
target_include_directories(astrumSnake PRIVATE astrum)
target_link_libraries(astrumSnake astrum)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
#	Haven't written any tests yet
#	So testing code is irrelevant

#	enable_testing()
#	find_package(GTest REQUIRED)
#	add_executable(astrumTests tests/test.cpp)
#	add_dependencies(astrumTests astrum)
#	target_include_directories(astrumTests PRIVATE astrum)
#	target_link_libraries(astrumTests astrum)
#	target_link_libraries(astrumTests GTest::GTest GTest::Main)
#	add_test(NAME astrumRunTests COMMAND astrumTests)

	find_package(Doxygen)
	set(DOXYGEN_EXTRACT_ALL YES)
	doxygen_add_docs(astrumDocs include ALL)
endif(CMAKE_BUILD_TYPE MATCHES "Debug")
