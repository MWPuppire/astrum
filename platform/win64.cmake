# The target operating system and processor
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Set flags for the source files to know which platform
string(APPEND CMAKE C_FLAGS "-DWIN_64 -mwindows")
string(APPEND CMAKE CXX_FLAGS "-DWIN_64 -mwindows")

# The compilers for C and C++
set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")

# The location of the target environment
set(CMAKE_FIND_ROOT_PATH /opt/homebrew/Cellar/mingw-w64/10.0.0_1/toolchain-x86_64/x86_64-w64-mingw32)

# Use host programs to execute properly
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Use headers and libraries from the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
