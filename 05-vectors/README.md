# 05 - Lists that know their type

## Goal

Fight three monsters in a row, and keep the loot they drop in an inventory.

## The new idea

- **`std::vector<T>` is a list that holds one type.** A JavaScript array
  holds anything: `[1, "two", true]`. A `std::vector<std::string>` holds
  strings and only strings. The type goes in the angle brackets `< >`.
- **The everyday moves look familiar:**

  | JavaScript | C++ |
  |---|---|
  | `let bag = ["bread"];` | `std::vector<std::string> bag{"bread"};` |
  | `bag.push("rope");` | `bag.push_back("rope");` |
  | `bag.length` | `bag.size()` |
  | `bag[0]` | `bag[0]` |
  | `for (const item of bag)` | `for (const auto& item : bag)` |

- **C++ does not check `[ ]` for you.** Ask JavaScript for an item that is
  not there and it says `undefined`. C++ does not check at all. This
  lesson's "Break it" shows what that means, and a tool that catches it.

## Try it

Add the new parts to your `my-arena/main.cpp`. The heart of it:

```cpp
std::vector<std::string> inventory{"bread"};
const std::vector<std::string> monsters{"Goblin", "Wolf", "Goblin Chief"};

for (const auto& monsterName : monsters) {
    // ... the same fight loop as lesson 04 ...
    inventory.push_back(loot);
}
```

Build and run with the same commands as lesson 01. A sample game:

```
A Goblin appears!

Ayla HP 60   Goblin HP 30
(a)ttack, (h)eal or (i)nventory? i
You carry 1 item(s):
  - bread
...
The Goblin drops a gold coin.

A Wolf appears!
...
Victory! Ayla cleared the arena with 4 items:
  - bread
  - gold coin
  - goblin ear
  - rusty sword
```

## What just happened

1. The outer `for` loop walks through `monsters`, one name at a time. Each
   pass is one whole fight.
2. When a monster falls, `lootTable[rollDice(0, 3)]` picks a random item by
   position (0 to 3, because the table has 4 items), and `push_back` adds
   it to the end of `inventory`.
3. Pressing `i` prints the bag with a range-for. `continue` jumps back to the
   top of the `while` loop, so looking is a free action.

About `const auto&`: `auto` lets the compiler fill in the type, `const` means
"only look", and `&` means "do not make a copy". Lesson 10 explains `&`
properly. For now, read it as "look at each one".

## Break it

(This lesson is on the long side. If you need a pause, this is the place:
the Break it works fine as a second sitting.)

Open `break-it.cpp` (build it in the lesson folder). It has 3 gold pouches
(positions 0, 1, 2) and reads `goldPouches[3]`.

**Step 1: no safety net.** Build it with the usual command and run it. On
our Linux machine it printed:

```
Pouch 0 holds 10 gold
Pouch 3 holds 0 gold
```

No error, and a made-up number. The program read memory that does not
belong to the vector and carried on. C++ has a name for this: **undefined
behavior** (UB). It means the rules of C++ say nothing about what happens
next. You may get junk, a crash, or a program that "works on my machine"
and fails on your friend's. Your number may differ from ours. That is
exactly the problem: nobody knows.

**Step 2: the safety net.** Build it again with two new flags:

**Windows (Developer PowerShell):**

```
cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address break-it.cpp
.\break-it.exe
```

**Linux / macOS:**

```
g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined break-it.cpp -o break-it
./break-it
```

`/Zi` (Windows) and `-g` (Linux/macOS) keep "debug info", so error reports
can name your file and line. `/fsanitize=address` and
`-fsanitize=address,undefined` turn on **AddressSanitizer** ("ASan"), a
guard that watches every memory read and write while the program runs.
(The `undefined` part adds a second checker for some other kinds of
undefined behavior.) With g++ on Linux (trimmed, folder paths shortened):

```
==ERROR: AddressSanitizer: heap-buffer-overflow on address 0xffffb24007bc
READ of size 4 at 0xffffb24007bc thread T0
    #0 0xaaaab2815888 in main break-it.cpp:12

0xffffb24007bc is located 0 bytes to the right of 12-byte region
```

Read it from the top: **what** (heap-buffer-overflow, a read past the end),
**where** (`main`, `break-it.cpp` line 12), and a hint: the vector owns a
12-byte region (3 ints of 4 bytes) and you read right after it. On Windows
the report looks similar but not identical.

**From now on, build everything with these flags**, the Arena too.
Programs run a bit slower with the guard on. That is fine while learning.
(Windows: if the program will not start, see "Common first-day problems" in
lesson 00.)

Lesson: **C++ does not check bounds unless you ask. Keep the sanitizer on
while you learn.**

## Try it yourself

1. Add a fourth monster, `"Troll"`, to `monsters`. Notice that you changed
   one line and the game grew.
2. Add `"silver ring"` to `lootTable`. The game never drops it. Why? Fix the
   `rollDice(0, 3)` call. (Hint: what is the last position now?)
3. Start the hero with three items instead of one.

## Going further

- **`.at()` checks for you.** Change `goldPouches[3]` to
  `goldPouches.at(3)` and rebuild. `.at()` checks the position and stops the
  program with a clear message. With g++:

  ```
  terminate called after throwing an instance of 'std::out_of_range'
    what():  vector::_M_range_check: __n (which is 3) >= this->size() (which is 3)
  ```

  That is an *exception*, like a JavaScript `throw` that nobody catches.
  **Python bridge:** it is C++'s version of Python's `IndexError`.
- `inventory.pop_back()` removes the last item. Removing one from the middle
  uses `erase`, which lesson 07 shows.
- When an error about a `std::vector` fills your screen with angle brackets,
  **scroll to the first line that mentions your own file**. That is where to
  look.
- Unreal's version of `std::vector` is `TArray`.

**Checkpoint: you can now** keep a list of things in a `std::vector`, loop
over it, and catch an out-of-bounds read with the sanitizer.

Next: [06-strings](../06-strings/) - type real commands like `use potion`.
