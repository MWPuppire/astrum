cmake_minimum_required(VERSION 3.10)

project(astrum VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -Wall -Wextra -Wshadow -fno-omit-frame-pointer -fsanitize=address -g")
string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address -g")

if(EMSCRIPTEN)
	add_definitions("-s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2")
else()
	find_package(SDL2 REQUIRED)
	find_package(SDL2TTF REQUIRED)
	find_package(SDL2IMAGE REQUIRED)
	find_package(OpenGL REQUIRED)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/SDL-gpu)

add_library(astrum)
target_sources(astrum PRIVATE src/astrum.cpp src/font.cpp src/graphics.cpp src/keyboard.cpp src/math.cpp src/mouse.cpp src/window.cpp src/event.cpp src/util.cpp src/image.cpp)
target_include_directories(astrum PUBLIC include/)
target_include_directories(astrum PUBLIC ${PROJECT_BINARY_DIR}/SDL_gpu/include)
target_link_libraries(astrum PUBLIC ${PROJECT_BINARY_DIR}/SDL_gpu/lib/libSDL2_gpu.a)
if(NOT EMSCRIPTEN)
	target_include_directories(astrum PUBLIC ${SDL2_INCLUDE_DIRS})
	target_include_directories(astrum PUBLIC ${OPENGL_INCLUDE_DIRS})
	target_include_directories(astrum PUBLIC ${SDL2TTF_INCLUDE_DIR})
	target_include_directories(astrum PUBLIC ${SDL2IMAGE_INCLUDE_DIR})
	target_link_libraries(astrum PUBLIC ${SDL2_LIBRARIES})
	target_link_libraries(astrum PUBLIC ${OPENGL_gl_LIBRARY})
	target_link_libraries(astrum PUBLIC ${SDL2TTF_LIBRARY})
	target_link_libraries(astrum PUBLIC ${SDL2IMAGE_LIBRARY})
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
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
endif()
