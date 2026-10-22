# unreal101

A hands-on path for learning **C++**, one small step at a time, and then using
it to build things in **Unreal Engine 5**.

The path is 25 short lessons (15-30 minutes each, plus a longer capstone):

1. **Part 1: C++ fundamentals (lessons 01-12).** Plain C++ in a terminal. You
   write small programs, compile them, break them, and fix them.
2. **Part 2: C++ in Unreal Engine (lessons 13-24).** You take what you learned
   and use it to make a game, using Unreal's C++ API.
3. **Capstone (lesson 25).** Finish and package your own game.

This is the third repo in a series:
[ai101](https://github.com/patdeg/ai101) (calling LLMs) and
[hello-docker](https://github.com/patdeg/hello-docker) (Docker). Same style:
numbered lessons, each one self-contained, each one building on the last.

## Who this is for

A high-school student who already knows a little JavaScript: variables,
`if`, loops, functions, arrays, objects. You do **not** need to know anything
about C++, memory, or game engines.

## Prerequisites

- A **Windows PC** with Visual Studio 2022 Community ("Game development with
  C++" workload). Part 1 uses the same compiler (MSVC) that Unreal uses.
  Linux/macOS with `g++` or `clang++` works for Part 1 only.
- A text editor (Visual Studio or VS Code).
- For Part 2: Unreal Engine 5.8. Lesson 13 comes with a prep sheet; start the
  download the day before, because it is large.

## The learning path

> **Status: curriculum in design.** The lesson list is being worked out and
> will appear here once it is settled. Lessons are added one folder at a time.

## Posters: print and memorize

Cheat-sheets for the critical syntax, themed around making games. White
background, made for inkjet printers.

- [`posters/unreal101-cheatsheet-superb-13x19.pdf`](posters/unreal101-cheatsheet-superb-13x19.pdf):
  the full poster, Super B (13 x 19 in).
- [`posters/unreal101-cheatsheet-letter.pdf`](posters/unreal101-cheatsheet-letter.pdf):
  the pocket edition, US Letter (8.5 x 11 in).

Print at 100% ("actual size"), not "fit to page".

## How each lesson is laid out

Each numbered folder is self-contained:

- `README.md` explains the goal, the new ideas, and why they matter.
- Source files you can build and run.
- "Try it" and "Break it" exercises. Reading a compiler error is a skill, so
  you will cause errors on purpose.

Do the lessons in order.

## License

To be decided before the first lesson is published.
