# 14 - Many files

## Goal

Move `Fighter` and `rollDice` out of `main.cpp` into files of their own.

## The new idea

`main.cpp` is now over 300 lines. Real programs have thousands of files.
C++ splits each piece of code into **two** files:

- **The header (`Fighter.h`) declares *what* exists:** the class, its
  fields, its function names and types. No bodies (except tiny one-liners).
- **The source file (`Fighter.cpp`) defines *how* it works:** the bodies.
- **`#include "Fighter.h"` pastes the header in**, word for word, so
  `main.cpp` knows what a `Fighter` is. Quotes for your files, angle
  brackets for C++'s own (`<string>`).
- **`#pragma once`** at the top of a header stops it from being pasted
  twice into the same file.

Think of a restaurant. The header is the **menu**: it lists every dish. The
`.cpp` is the **kitchen**: it actually cooks them. Anyone can read the menu
and order; only the kitchen needs to know the recipe.

**JS bridge:** this is like ES modules, where `export` lists what a file
offers and `import` uses it. In C++ the "export list" is a separate file you
write by hand. Every Unreal class is exactly this: a `.h` and a `.cpp`.

## Try it

**From this lesson on, the Arena is several files.** Here is the easiest way
to keep up: copy this lesson's files (`main.cpp`, `Dice.h`, `Dice.cpp`,
`Fighter.h`, `Fighter.cpp`) into your `my-arena` folder, replacing your
`main.cpp`. Then **read** them, and in later lessons type only the parts
each README shows. Today's lesson is about *where* code lives, not new
code: almost every line already exists in your lesson 13 version.

What changes in your Arena: `rollDice` moves to `Dice.h` / `Dice.cpp`;
`Stance`, `stanceName` and the `Fighter` class move to `Fighter.h` /
`Fighter.cpp`; `main.cpp` loses those parts and gains two
`#include "..."` lines.

Read the five files in this order:

| File | What is in it |
|---|---|
| `Dice.h` | one line: `int rollDice(int min, int max);` |
| `Dice.cpp` | the dice machine's body |
| `Fighter.h` | `Stance`, `stanceName`, and the `Fighter` class (declarations) |
| `Fighter.cpp` | the bodies of the constructor, `takeDamage`, `heal`, ... |
| `main.cpp` | the game, now starting with `#include "Fighter.h"` |

Outside the class, a member function's name gets the class in front:

```cpp
int Fighter::takeDamage(int amount) {   // "Fighter's takeDamage"
    // ...
}
```

Build by listing **every `.cpp` file**. Headers are never listed; they come
in through `#include`.

**Windows:**

```
cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp Fighter.cpp Dice.cpp
.\main.exe
```

**Linux / macOS:**

```
g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp Fighter.cpp Dice.cpp -o arena
./arena
```

The game plays exactly as in lesson 13. Nothing changed for the player.
That is the point: this was a tidy-up.

## What just happened

Building is really **two** jobs, and you just saw both:

1. **The compiler** reads each `.cpp` file **alone**, one at a time. When it
   reads `main.cpp` it sees `rollDice(0, 3)`, checks the menu (`Dice.h`), and
   says "fine, that exists somewhere". It does not look for the body.
2. **The linker** takes the three compiled pieces and connects every call
   to its body: the call in `main.cpp` gets wired to the kitchen in
   `Dice.cpp`. Its output is the program: `main.exe` on Windows, `arena` on
   Linux/macOS (the name after `-o`).

```
  main.cpp ----compiler----> main.obj ---+
  Fighter.cpp --compiler---> Fighter.obj +--linker--> main.exe
  Dice.cpp ----compiler----> Dice.obj ---+
```

(On Linux and macOS the pieces end in `.o` instead of `.obj`.) Those `.obj`
files are the "half-built pieces" you saw in lesson 01. Until now, both
jobs ran inside one command, and you never noticed. Today you meet the
linker in person.

## Break it

**First, the small one.** Open `break-it.cpp`. The class declares
`takeDamage`, but nobody ever writes its body. Build it in the lesson
folder. With g++ on Linux (folder path shortened):

```
/usr/bin/ld: /tmp/ccZcipxo.o: in function `main':
break-it.cpp:19: undefined reference to `Fighter::takeDamage(int)'
collect2: error: ld returned 1 exit status
```

Look at who is talking: `ld` is the **linker**, not the compiler. The
compiler was perfectly happy, because the declaration promised that the
function exists. You can prove it: compile only, without linking, using
`-c` (Windows: `/c`):

```
g++ -std=c++20 -Wall -Wextra -c break-it.cpp
```

No error at all. On Windows the linker error has an `LNK` code instead of a
`C` code (`LNK2019`, "unresolved external symbol"). The menu lists a dish,
and no kitchen cooks it.

**Then, the real-life one.** In `my-arena`, build the Arena but forget a
file:

```
g++ -std=c++20 -Wall -Wextra -g main.cpp Fighter.cpp -o arena
```

```
main.cpp:78: undefined reference to `rollDice(int, int)'
main.cpp:238: undefined reference to `rollDice(int, int)'
...
```

Same message, many times. In real projects this is the most common linker
error: a `.cpp` file that is not part of the build.

Lesson: **compiler errors and linker errors are different kinds of
problem.** A compiler error (`C....`, or `error:` with a line of your code)
means "this line is wrong". A linker error (`LNK....`, or `undefined
reference`) means "I can't find the body of something you promised".
Check three things: is it defined? Is its `.cpp` in the build? Do the
names and types match exactly?

## Try it yourself

1. Move `printStatus` and `anyAlive` out of `main.cpp` into the Fighter
   files. Which part goes in `.h`, which in `.cpp`?
2. In `Fighter.cpp`, change `int Fighter::takeDamage(int amount)` to
   `int Fighter::takeDamage(double amount)`. Read the error. Is it the
   compiler or the linker this time?
3. Remove `#pragma once` from `Fighter.h` and `#include "Fighter.h"` twice in
   `main.cpp`. Read the error, then put it back.

## Going further

- Typing every `.cpp` name gets old. Build tools list them for you: the
  optional [appendix-cmake](../appendix-cmake/) shows CMake, and Unreal has
  its own build tool that finds your files automatically.
- `#pragma once` is not part of standard C++, but every major compiler
  supports it, and Unreal uses it, so it is our default. The standard,
  older way is an **include guard**. You will meet it in older code:

  ```cpp
  #ifndef FIGHTER_H
  #define FIGHTER_H
  // ... the whole header ...
  #endif
  ```

  The first time, `FIGHTER_H` is not defined, so the header is read and
  defines it. The second time, `#ifndef` skips everything.

**Checkpoint: you can now** split a program into headers and source files,
build them together, and tell a compiler error from a linker error.

Next: [15-inheritance-and-virtual](../15-inheritance-and-virtual/) - Goblins,
Trolls and Slimes, each fighting its own way.
