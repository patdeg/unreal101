# 16 - Functions as values

## Goal

Make the fastest monster act first, by sorting the wave with a function you
write on the spot.

## The new idea

- **A lambda is a function without a name, written where you need it.** You
  already know them from JavaScript as arrow functions:

  | | JavaScript | C++ |
  |---|---|---|
  | is it alive? | `(m) => m.isAlive()` | `[](const Monster* m) { return m->isAlive(); }` |
  | sort fastest first | `wave.sort((a, b) => b.speed - a.speed)` | `std::sort(wave.begin(), wave.end(), [](const Monster* a, const Monster* b) { return a->getSpeed() > b->getSpeed(); });` |

  Longer, because C++ wants the input types and a `return`, but the same
  idea. Note the sort row: JavaScript's `sort` wants a **number** (negative,
  zero or positive), while C++'s `std::sort` wants **true or false**:
  "should `a` come before `b`?" **Python bridge:**
  `sorted(wave, key=lambda m: m.speed, reverse=True)`.
- **`const Monster*` is a pointer you may only look through.** It is the
  pointer version of `const Monster&` (lesson 10): you can call `const`
  functions like `getSpeed()`, but not change the monster.
- **The `[ ]` part is new: the capture list.** It says which local
  variables from outside the lambda it may use. `[kills]` gives it a
  **copy** of `kills`. `[&kills]` gives it a **reference** to the real one.
  Same rule as lesson 10: a copy unless you say `&`. You will also meet
  shortcuts: `[&]` means "everything I use, by reference", `[=]` means "by
  copy", and `[this]` lets a lambda inside a class use the object's
  members. Unreal code uses those a lot. Listing names one by one, as here,
  is the clearest.
- **A lambda is a value.** You can pass it to a function (like `std::sort`)
  or store it in a variable with `auto` and call it later.

## Try it

### What changes in your Arena

Only `main.cpp` changes (the other files are the same as lesson 15): the
wave is built in a different order and then sorted by speed with a lambda;
a second lambda, `onMonsterDown`, replaces the loot code in the attack loop
and counts kills; the final messages show the kill count.

### The two lambdas

Add the new parts to `my-arena/main.cpp`. First, the sort, right after the
wave is made:

```cpp
std::sort(wave.begin(), wave.end(), [](const Monster* a, const Monster* b) {
    return a->getSpeed() > b->getSpeed();
});
```

`std::sort` (from `<algorithm>`) calls your lambda over and over with two
monsters and asks: "should `a` come before `b`?" Answer `true` and `a` goes
first.

Then a lambda stored in a variable, which runs whenever a monster falls:

```cpp
int kills = 0;
auto onMonsterDown = [&kills, &inventory, &lootTable](const Monster& monster) {
    ++kills;
    // ... drop loot, print a message ...
};

// later, in the attack code:
onMonsterDown(*front);
```

Build with all four `.cpp` files, as in lesson 15:

**Windows:**

```
cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp Fighter.cpp Monster.cpp Dice.cpp
.\main.exe
```

**Linux / macOS:**

```
g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp Fighter.cpp Monster.cpp Dice.cpp -o arena
./arena
```

The wave was created as Troll, Goblin, Slime. After the sort:

```
A Troll, a Goblin and a Slime enter the arena!
Turn order: Goblin (8) Slime (4) Troll (2)
...
Ayla hits Goblin for 10!
The Goblin falls and drops a goblin ear. (Kills: 1)
```

## What just happened

You handed `std::sort` a small piece of *behavior*, not data. `std::sort`
knows how to sort anything; you told it what "faster" means. That pattern,
"a general tool plus a small function from you", is everywhere in C++ and in
Unreal.

The second lambda captured three outside variables by reference. Each
time it runs, it changes the **real** `kills` and the **real**
`inventory`.

## Break it

Open `break-it.cpp`. Its kill-counter lambda captures `[kills]`, by copy.
(It also says `mutable`, which only means "this lambda may change its own
copy". Without it, `++kills` would not even compile.) Build and run it in
the lesson folder. With g++ on Linux:

```
  Goblin falls. (Kills inside the lambda: 1)
  Troll falls. (Kills inside the lambda: 2)
  Slime falls. (Kills inside the lambda: 3)
Total kills: 0
```

Inside the lambda the count goes up nicely. Outside, it is still 0. The
lambda counted on its own **copy** of `kills`, made when the lambda was
created. The real `kills` was never touched. No error, no warning. (Windows
behaves the same: this is how captures are defined.)

Change the capture to `[&kills]` and remove `mutable`. Rebuild:

```
Total kills: 3
```

Lesson: **a capture is a copy unless you say `&`.** It is lesson 10's rule
again, wearing a new hat.

## Try it yourself

1. Print the party sorted by HP, lowest first, **without moving anyone**.
   Make a `std::vector<const Fighter*>` holding `&party[0]` and `&party[1]`,
   sort *that* with a lambda, and print through the pointers. Why not just
   sort `party` itself? Because monsters point at party *slots* (lesson
   13). Sorting would move the fighters between slots, and every monster
   would silently switch to whoever now sits in its slot. No crash, no
   warning, wrong game.
2. Use `std::count_if(wave.begin(), wave.end(), [](const Monster* m) {
   return m->isAlive(); })` to print "3 monsters left" each round.
3. Store a lambda in `std::function<void(const Monster&)> onDeath;`
   (`#include <functional>`), and call it when a monster falls. A
   `std::function` is a box that can hold *any* function with that shape.
   That is the idea behind Unreal's delegates (lesson 31).

## Going further

Why not write it the JavaScript way, `return a->getSpeed() - b->getSpeed();`?
It compiles without a single warning. But the number gets turned into
`true` or `false` (anything that is not 0 is `true`), so the answer no
longer means "a goes first". On our machine, speeds 2, 8, 4 came out as
`4 8 2`: not sorted at all. A wrong comparator is undefined behavior, and on a
long list it can even crash `std::sort`. Always return a real comparison
with `<` or `>`.

**Checkpoint: you can now** write a lambda, pass it to `std::sort`, store
one in a variable, and choose between capture by copy and by reference.

Next: [17-destructors-and-raii](../17-destructors-and-raii/) - cleanup code
that always runs, however a round ends.
