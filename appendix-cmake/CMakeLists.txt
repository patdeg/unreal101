# CMakeLists.txt - the build recipe for the Text Arena (lesson 18 version).
# Put this file in the same folder as main.cpp, Fighter.cpp, and friends.

# Which CMake features this file needs.
cmake_minimum_required(VERSION 3.20)

# The project's name, and that it is written in C++.
project(TextArena LANGUAGES CXX)

# Use C++20, and refuse to build with anything older.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The program to build ("arena") and every .cpp file that goes into it.
# Headers are not listed: #include finds them, as always.
add_executable(arena
    main.cpp
    Fighter.cpp
    Monster.cpp
    Spawner.cpp
    Dice.cpp
)

# The same warning flags you have used all along.
if(MSVC)
    target_compile_options(arena PRIVATE /W4 /EHsc)
else()
    target_compile_options(arena PRIVATE -Wall -Wextra)
endif()
