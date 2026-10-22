# Appendix - Building with CMake (optional)

## Goal

Build the Text Arena with one short recipe file instead of a long `cl` or
`g++` line.

This page is **optional**. Part 2 does not need it, because Unreal has its
own build tool. Do it after lesson 18 if you are curious, or skip it.

## The new idea

- **A build tool remembers how to build your program.** Since lesson 14
  you have typed every `.cpp` file by hand, plus all the flags. Forget one
  file and you get a linker error. A build tool keeps that list in a file.
- **CMake is the most common one for C++.** You write a `CMakeLists.txt`
  that says: "a program called `arena`, made from these files, in C++20,
  with these warnings". CMake then drives the real compiler for you: MSVC on
  Windows, g++ or clang on Linux and macOS.
- **JS bridge:** it is like the `scripts` in a `package.json` plus a bundler
  config: one file that says how the project is built, so nobody has to
  remember the command.

## Try it

You need CMake installed. On Windows it comes with Visual Studio's "Desktop
development with C++" workload, so it already works in Developer PowerShell.
On Linux or macOS, install `cmake` with your package manager.

1. Go to your `my-arena` folder: it already holds the lesson 18 Arena
   (`main.cpp`, `Dice.h`, `Dice.cpp`, `Fighter.h`, `Fighter.cpp`,
   `Monster.h`, `Monster.cpp`, `Spawner.h`, `Spawner.cpp`).
2. Copy this folder's `CMakeLists.txt` into `my-arena`. The important lines:

   ```cmake
   cmake_minimum_required(VERSION 3.20)
   project(TextArena LANGUAGES CXX)
   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   add_executable(arena main.cpp Fighter.cpp Monster.cpp Spawner.cpp Dice.cpp)
   ```

   (The real file lists one file per line and adds the warning flags.)
3. Build in two steps, from that folder:

   ```
   cmake -S . -B build
   cmake --build build --config Debug
   ```

   The first command **configures**: it looks at your computer, finds a
   compiler, and writes the real build files into a new `build` folder. The
   second command **builds**. Next time you change code, only the second
   command is needed. (`--config Debug` matters on Windows, where one build
   folder can hold several configurations; elsewhere it does no harm.)

On Linux, the build step prints:

```
[ 16%] Building CXX object CMakeFiles/arena.dir/main.cpp.o
[ 33%] Building CXX object CMakeFiles/arena.dir/Fighter.cpp.o
[ 50%] Building CXX object CMakeFiles/arena.dir/Monster.cpp.o
[ 66%] Building CXX object CMakeFiles/arena.dir/Spawner.cpp.o
[ 83%] Building CXX object CMakeFiles/arena.dir/Dice.cpp.o
[100%] Linking CXX executable arena
[100%] Built target arena
```

Run it with `./build/arena`. On Windows, CMake normally uses Visual Studio's
own build system, which puts the program in a folder named after the
configuration: `.\build\Debug\arena.exe`.

## What just happened

You can see both jobs from lesson 14 in that output: five lines of
**compiling** (one per `.cpp` file) and one line of **linking**. Change
only `Monster.cpp` and build again: CMake recompiles just that one file and
links again. On a big project, that saves minutes.

The `build` folder is throwaway. You can delete it at any time and run both
commands again. Never put it in git (this repo's `.gitignore` already skips
it).

## Break it

Delete the `Fighter.cpp` line from `add_executable` and build again. With
g++ on Linux (trimmed):

```
/usr/bin/ld: CMakeFiles/arena.dir/main.cpp.o: in function `healParty(...)':
main.cpp:(.text+0x48): undefined reference to `Fighter::heal(int)'
```

The same linker error as lesson 14, now coming through CMake. A build tool
only knows the files you list. (On Windows you get the `LNK2019` error.)
Put the line back.

## Try it yourself

1. Add the sanitizer back. On Linux and macOS: add
   `-fsanitize=address,undefined` to `target_compile_options`, plus a new
   line `target_link_options(arena PRIVATE -fsanitize=address,undefined)`.
   On Windows: add `/fsanitize=address` to the MSVC
   `target_compile_options`.
2. Add a second program to the same recipe: copy `pointers-basics.cpp` from
   lesson 13 into your folder, add the line
   `add_executable(pointers pointers-basics.cpp)`, and build. You now get
   two programs from one `cmake --build build`.

## Going further

Unreal does not use CMake. Each Unreal module has a `.Build.cs` file that
plays the same role, and the Unreal build tool reads it. Same idea, different
recipe.

**Checkpoint: you can now** describe a C++ program in a `CMakeLists.txt` and
build it with two CMake commands.

Back to: [18-unique-ptr](../18-unique-ptr/), or on to
[19-unreal-hello](../19-unreal-hello/).
