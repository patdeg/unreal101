# 06 - Working with text

## Goal

Let the player name the hero and type whole commands like `use potion`.

## The new idea

- **`std::getline(std::cin, line)` reads a whole line**, spaces included, up
  to Enter. `std::cin >> word` stops at the first space. That difference is
  this lesson's "Break it". (`getline` is the closest thing to JavaScript's
  `prompt()` or Python's `input()`.)
- **Cutting text up works like JavaScript, with new names:**

  | JavaScript | C++ |
  |---|---|
  | `line.indexOf(" ")` | `line.find(' ')` |
  | returns `-1` if not found | returns `std::string::npos` if not found |
  | `line.substring(0, 3)` | `line.substr(0, 3)` (start, **count**) |
  | `line.length` | `line.size()` |
  | `a === b`, `a + b` | `a == b`, `a + b` |

- **`npos` means "not found".** It is a special, very large number. Always
  compare with `std::string::npos`, never with `-1`.

## Try it

Add the new parts to your `my-arena/main.cpp`. The key piece splits a line
into a verb and an object:

```cpp
std::string verb = line;
std::string object;
std::size_t space = line.find(' ');
if (space != std::string::npos) {
    verb = line.substr(0, space);     // "use"
    object = line.substr(space + 1);  // "potion"
}
```

(`std::size_t` is the type C++ uses for sizes and positions: a whole number
that is never negative.)

Build and run, with the sanitizer flags from lesson 05 as always now:

**Windows:**

```
cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp
.\main.exe
```

**Linux / macOS:**

```
g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp -o main
./main
```

A sample game:

```
Name your hero: Sir Gob

A Goblin appears!

Sir Gob HP 60   Goblin HP 30
> look
A Goblin with 30 HP glares at Sir Gob.

Sir Gob HP 60   Goblin HP 30
> use sword
You can't use 'sword' here.

Sir Gob HP 60   Goblin HP 30
> dance
Unknown command 'dance'. Try: attack, use potion, look, inventory

Sir Gob HP 60   Goblin HP 30
> attack
Sir Gob hits for 9!
Goblin hits for 7!
```

## What just happened

1. `getline` put the whole typed line into `line`, for example
   `"use potion"`. Like `std::cin >> choice` in lesson 03, it also answers
   "did the read work?", so `if (!std::getline(std::cin, line))` means "if
   the input has ended". For the hero's name,
   `if (!std::getline(std::cin, heroName) || heroName.empty())` means: "if
   reading failed, or the player just pressed Enter, use the default
   name".
2. `find(' ')` found the space at position 3. `substr(0, 3)` cut out `"use"`
   and `substr(4)` (no count means "to the end") cut out `"potion"`.
3. A chain of `if (verb == "...")` decided what to do. Strings compare with
   `==`, just like in JavaScript.
4. Commands that are only information (`look`, `inventory`, a typo) use
   `continue`, so the monster does not get a free hit.

## Break it

Open `break-it.cpp`. It reads the name with `std::cin >> name;` instead of
`getline`. Build it in the lesson folder, run it, and type `Sir Gob` as the
name.

With g++ on Linux:

```
Name your hero: Sir Gob
First command:
Hero name: [Sir]
Command:   [Gob]
```

It never waited for a command! `>>` read one word, `Sir`, and stopped at
the space. `Gob` stayed waiting in the input, and the next `>>` grabbed it
straight away. No error, no warning, just wrong. (Windows behaves the same:
this is how `>>` is defined, not a compiler quirk.)

Change both reads to `std::getline(std::cin, name);` and
`std::getline(std::cin, command);`, rebuild, and try `Sir Gob` again.

Lesson: **`>>` reads one word. `getline` reads a line.**

## Try it yourself

1. Add a `help` command that prints the list of commands. Should it cost a
   turn?
2. Make `a` a shortcut for `attack`.
3. Make `look` also print how many items you carry, using
   `inventory.size()`.

## Going further

- Mixing `>>` and `getline` in one program has a trap: `>>` leaves the Enter
  key's newline behind, and the next `getline` reads an empty line. That is
  why this program uses `getline` for everything.
- Players will type `ATTACK` too. `std::tolower` (from `<cctype>`) can
  lower-case each character before you compare.
- Unreal's string type is `FString`. Its `Find`, `Mid` and `Split` do the same
  jobs as `find` and `substr`.

**Checkpoint: you can now** read a whole line of text, cut it into words,
and compare strings.

Next: [07-maps](../07-maps/) - give every piece of loot a price, and sell it.
