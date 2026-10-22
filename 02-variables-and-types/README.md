# 02 - Every value has a type

## Goal

Store your hero's stats in variables and print a stat card.

## The new idea

- **Every variable has a type, and it never changes.** In JavaScript,
  `let hp = 100;` can later hold `"banana"`. In C++ you write the type first:
  `int hp = 100;`, and `hp` is a whole number forever.
- **The four types you need for now:**
  - `int` - whole numbers (`100`, `-5`)
  - `double` - numbers with decimals (`0.25`)
  - `bool` - `true` or `false`
  - `std::string` - text (`"Ayla"`). Add `#include <string>` first.
- **`const` means "never changes".** After `const int maxHp = 100;`, the
  compiler stops anyone who tries to change `maxHp`. (Like JS `const`, but
  stricter: in JS a `const` object can still have its fields changed.)

## Try it

In your `my-arena` folder, replace `main.cpp` with the program from this
lesson's `main.cpp` (type it). The heart of it:

```cpp
const std::string heroName = "Ayla";
int hp = 100;
int attack = 12;
double critChance = 0.25;
bool hasShield = true;
```

Build and run it in `my-arena` with the same commands as lesson 01. You
should see:

```
=== Ayla ===
HP:     100
Attack: 12
Crit:   25%
Shield: true
7 / 2 = 3   (not 3.5!)
7 / 2.0 = 3.5
```

## What just happened

You gave each stat a type, and `<<` printed each one. The last two lines
show a classic trap: when **both** numbers are `int`, the answer is also an
`int`, so `7 / 2` is `3`, not `3.5`. Make one side a `double` (`2.0`) and
you get `3.5`. JavaScript has only one kind of number, so you never hit this
there.

**Python bridge:** in Python 3, `7 / 2` is `3.5` too, and `7 // 2` is `3`.
C++ `int / int` acts like Python's `//`, with one twist for negative
numbers: C++ cuts toward zero (`-7 / 2` is `-3`), while Python rounds down
(`-7 // 2` is `-4`).

## Break it

Open `break-it.cpp`. It tries `int hp = "lots";`. Build it in the lesson
folder (`cd ../02-variables-and-types`). With g++ you will see:

```
break-it.cpp: In function 'int main()':
break-it.cpp:5:14: error: invalid conversion from 'const char*' to 'int' [-fpermissive]
    5 |     int hp = "lots";
      |              ^~~~~~
      |              |
      |              const char*
```

(Visual Studio says something similar, with an error code starting with
`C`.) You tried to put text into a number-only box. A type is a **promise**,
and the compiler enforces it. (`const char*` is how C++ names a piece of
text in quotes; lesson 13 explains the `*`.)

Ignore the `[-fpermissive]` tag at the end. It names a compiler switch that
would turn this error into a warning. Never use it: the error is right.

Now cause a similar error with `double critChance = "high";` and notice that
you can already read it. (Careful: `bool shield = "yes";` is **not** an
error. C++ quietly turns the text into `true`. Types are strict, but not
perfect.)

Go back to `my-arena` (`cd ../my-arena`) when you are done.

## Try it yourself

1. Add a `mana` stat and print it.
2. Make a second hero, `"Bram"`, with different stats.
3. Try `heroName = "Zed";` on the `const` variable and read what the compiler
   says. Then remove `const` and see it work.

## Going further

You can write `auto hp = 100;` and the compiler works out `int` for you. It
is still a fixed type; `auto` just saves typing. Lesson 05 starts using it
where the type is long.

**Checkpoint: you can now** store numbers, text and true/false in typed
variables, and read a "wrong type" error.

Next: [03-decisions-and-loops](../03-decisions-and-loops/) - make the hero
fight a goblin.
