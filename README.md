# unreal101

A hands-on path for learning **C++**, one small step at a time, and then using
it to build a game in **Unreal Engine 5**.

There are 33 short lessons and a longer capstone. Most lessons take 15-30
minutes; the few that take longer say so.

1. **Part 1: C++ fundamentals (lessons 01-18).** Plain C++ in a terminal. You
   write small programs, compile them, break them, and fix them.
2. **Part 2: C++ in Unreal Engine (lessons 19-33).** You use what you
   learned to make a 3D game with Unreal's C++ API.
3. **Capstone (lesson 34).** Finish and package your own game.

This is the third repo in a series, after
[ai101](https://github.com/patdeg/ai101) (calling LLMs) and
[hello-docker](https://github.com/patdeg/hello-docker) (Docker). Same style:
numbered lessons, each one self-contained and building on the last.

## Who this is for

A high-school student who already knows a little JavaScript: variables,
`if`, loops, functions, arrays, objects. You do **not** need to know anything
about C++, memory, or game engines.

## Prerequisites

- A **Windows PC** with Visual Studio 2022 Community: the "Desktop
  development with C++" workload for Part 1, plus "Game development with
  C++" before lesson 19. Part 1 uses the same compiler (MSVC) as Unreal.
  Linux or macOS with `g++` or `clang++` works for Part 1 only.
- A text editor (Visual Studio or VS Code).
- For Part 2: Unreal Engine 5.8. Lesson 19 has a prep sheet. The download is
  large, so start it the day before.

## The learning path

Start with [00-start-here](00-start-here/): tool setup, how the lessons work,
and what to do when you get stuck. New words are explained in the
[glossary](GLOSSARY.md).

| # | Lesson | You make |
|---|---|---|
| 00 | [Start here](00-start-here/) | A working setup |
| 01 | [Hello, compiler](01-hello-compiler/) | Your first program, and your first error |
| 02 | [Every value has a type](02-variables-and-types/) | The hero's stat card |
| 03 | [Decisions and loops](03-decisions-and-loops/) | A fight with a goblin |
| 04 | [Functions with types](04-functions/) | Random damage with dice |
| 05 | [Lists that know their type](05-vectors/) | Three fights in a row, and an inventory |
| 06 | [Working with text](06-strings/) | Typed commands like `use potion` |
| 07 | [Look things up by name](07-maps/) | A loot price table and a `sell` command |
| 08 | [Your own types](08-structs/) | One `Fighter` type for heroes and monsters |
| 09 | [Named choices](09-enums-and-switch/) | Monsters that defend and get stunned |
| 10 | [Copies vs references](10-references-and-copies/) | A party of two, and healing that sticks |
| 11 | [Classes guard their data](11-classes/) | HP that can never go below zero |
| 12 | [Watching your program think](12-debugger/) | A planted bug, caught in the debugger |
| 13 | [Pointers, honestly](13-pointers/) | Monsters with a target, or nobody |
| 14 | [Many files](14-header-and-source/) | The Arena split into `.h` and `.cpp` files |
| 15 | [One interface, many monsters](15-inheritance-and-virtual/) | A Goblin, a Troll and a Slime, each fighting its own way |
| 16 | [Functions as values](16-lambdas/) | Turn order sorted by speed |
| 17 | [Cleanup that always happens](17-destructors-and-raii/) | A round banner that always closes |
| 18 | [Who owns this?](18-unique-ptr/) | Text Arena v1.0: waves of monsters made at runtime |
| - | [Optional: building with CMake](appendix-cmake/) | The Arena built from a `CMakeLists.txt` |
| 19 | [Unreal says hello](19-unreal-hello/) (with a [prep sheet](19-unreal-hello/PREP.md)) | Your C++ running inside the engine |
| 20 | [Your first Actor](20-your-first-actor/) | A spinning orb |
| 21 | [Knobs in the editor](21-uproperty/) | Orbs tuned without recompiling |
| 22 | [Actors are built from parts](22-components/) | An orb with a real mesh |
| 23 | [The player presses a key](23-input/) | A dash on Left Shift |
| 24 | [Something touched me](24-overlaps/) | Orbs you collect by touching |
| 25 | [The referee](25-game-mode/) | A score |
| 26 | [Unreal's containers](26-tarray-and-tmap/) | A points table and a win |
| 27 | [Typed, data-driven orbs](27-uenum-and-ustruct/) | Orb kinds tuned in one table |
| 28 | [Memory the Unreal way](28-unreal-memory/) | A tracker that never crashes |
| 29 | [Things appear over time](29-spawning-and-timers/) | Orb waves |
| 30 | [Something chases you](30-enemy-chase/) | An enemy that hunts the player |
| 31 | [Your own events](31-delegates/) | A score-changed event |
| 32 | [C++ and Blueprints together](32-blueprints-and-cpp/) | A collect effect in Blueprint |
| 33 | [A HUD on screen](33-umg-hud/) | The score on screen |
| 34 | [Capstone: Orb Arena 1.0](34-capstone/) | A finished, packaged game |
| - | [Bonus: line traces](bonus-line-trace/) | A scanner that highlights orbs |
| - | [Bonus: saving](bonus-save-game/) | A high score that survives |

The Unreal lessons are written for Unreal Engine 5.8 and have not yet been
run on a fresh install, so expect small differences, and please report them.

## Posters: print and memorize

Two one-page cheat sheets for the syntax you use most, themed around making
games. White background, made for inkjet printers, in two sizes each.

| Poster | Super B (13 x 19 in) | US Letter (8.5 x 11 in) |
|---|---|---|
| **C++ basics** (from `Hello, compiler` to lambdas) | [PDF](posters/unreal101-cheatsheet-cpp-superb-13x19.pdf) | [PDF](posters/unreal101-cheatsheet-cpp-letter.pdf) |
| **Unreal Engine C++** (Actors to Blueprints) | [PDF](posters/unreal101-cheatsheet-unreal-superb-13x19.pdf) | [PDF](posters/unreal101-cheatsheet-unreal-letter.pdf) |

Print at 100% ("actual size"), not "fit to page".

## How each lesson is laid out

Each numbered folder is self-contained:

- `README.md` explains the goal, the new idea, and why it matters.
- Source files you can build and run.
- "Try it" and "Break it" exercises. Reading a compiler error is a skill, so
  you cause errors on purpose.

Do the lessons in order.

## License

- **Code** (the `.cpp`, `.h` and `Source/` files) is under the [MIT License](LICENSE).
- **Lessons, text and posters** are under
  [Creative Commons Attribution 4.0](LICENSE-CONTENT) (CC BY 4.0). Use and
  adapt them freely, including for teaching, and credit "Patrick Deglon" with
  a link to this repository.
- Unreal Engine is a product of Epic Games and is covered by Epic's own
  license terms, not by this repository's license. These lessons only show how
  to use its API.
