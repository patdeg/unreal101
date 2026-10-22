# 18 - Who owns this?

## Goal

Create monsters while the game runs, in three waves, and have them cleaned
up automatically when they die. This is **Text Arena v1.0**.

This is the last of the three hard lessons, and the end of Part 1. It can
take closer to an hour, so take it in two sittings if you like: the new
idea and the Spawner first, then the waves, the Break it and the final
challenge.

## The new idea

Lesson 17 ended with a problem: objects made with `new` are never cleaned
up unless someone remembers `delete`. And in lesson 13, a pointer could
outlive its object. Both problems have the same root question: **who owns
this object?** Who is responsible for destroying it?

- **`std::unique_ptr<Monster>` is a pointer that owns its monster.** When
  the `unique_ptr` is destroyed (it goes out of scope, or is erased from a
  vector), it destroys the monster too. Its destructor does the `delete` for
  you. That is RAII from lesson 17, packed into a pointer.
- **`std::make_unique<Goblin>()` creates one.** It replaces `new`. You
  include `<memory>` to get both.
- **There is exactly one owner.** You cannot copy a `unique_ptr`, but you
  can **hand it over** with `std::move(owner)`. Afterwards the old variable
  is empty (`nullptr`).
- **Plain pointers still have a job: looking.** `unique_ptr` owns; a plain
  `Monster*` borrowed with `.get()` only looks, for a moment.

Think of the one key to a locker. Whoever holds the key owns the locker. You
can **give** the key to someone (`std::move`), but you cannot photocopy it.
When the key holder leaves the building, the locker is emptied. Friends can
still peek inside while the owner is around (a plain pointer).

**JS bridge:** JavaScript never asks who deletes an object; its garbage
collector cleans up eventually. `unique_ptr` gives you nearly the same
comfort without a garbage collector, and cleanup happens at an exact,
predictable moment. Unreal takes the other road and has a garbage collector
for its objects (lesson 28). Knowing both makes that lesson easier.

## Try it

### What changes in your Arena

- **New files `Spawner.h` and `Spawner.cpp`:** a `Spawner` that makes random
  monsters, one at a time or a whole wave.
- **`Monster.h` / `Monster.cpp`:** `Monster` gets a destructor that prints
  "fades away", and the Goblin gets one of its own that drops its dagger.
- **`Fighter.h`:** a new one-line `restoreFull()`, used for the rest between
  waves.
- **`main.cpp`:** three waves (1, then 2, then 3 random monsters) instead of
  one fixed fight. The fallen are erased at the end of each round. Between
  waves the party rests: everyone, even a fallen hero, gets back up at full
  HP, and the potion bag is refilled to 3. At the top of the file,
  `using Wave = std::vector<std::unique_ptr<Monster>>;` just gives that long
  type a short name.

Copy the new and changed files into `my-arena`, then read them.

### The spawner and the waves

The heart of the spawner:

```cpp
std::unique_ptr<Monster> Spawner::spawnRandom() {
    switch (rollDice(1, 3)) {
        case 1:
            return std::make_unique<Goblin>();
        // ... Troll, Slime
    }
}

std::vector<std::unique_ptr<Monster>> Spawner::makeWave(int size) {
    std::vector<std::unique_ptr<Monster>> wave;
    for (int i = 0; i < size; ++i) {
        std::unique_ptr<Monster> monster = spawnRandom();
        wave.push_back(std::move(monster));   // hand over the key
    }
    return wave;
}
```

In `main.cpp`, each wave owns its monsters, and the fallen are erased at the
end of each round:

```cpp
std::erase_if(wave, [](const std::unique_ptr<Monster>& monster) {
    return !monster->isAlive();
});
```

Erasing a `unique_ptr` destroys the monster it owns. Build with **five**
`.cpp` files now:

**Windows:**

```
cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp Fighter.cpp Monster.cpp Spawner.cpp Dice.cpp
.\main.exe
```

**Linux / macOS:**

```
g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp Fighter.cpp Monster.cpp Spawner.cpp Dice.cpp -o arena
./arena
```

```
=== Wave 1 of 3: Slime ===
...
--- Round 2 begins ---
Ayla hits Slime for 11!
Bram hits Slime for 9!
The Slime falls and drops a healing herb. (Kills: 1)
The Slime fades away.
--- Round 2 over ---

Wave 1 cleared! The party rests: everyone is back on their feet at full HP,
and the potion bag is refilled.

=== Wave 2 of 3: Slime Slime ===
...
=== Wave 3 of 3: Goblin Goblin Slime ===
...
--- Round 11 begins ---
Ayla hits Goblin for 13!
The Goblin falls and drops a goblin ear. (Kills: 4)
Bram hits Goblin for 9!
Goblin hits Bram for 4!
Goblin hits Bram for 4!
Slime hits Ayla for 4!
The Goblin's dagger clatters to the floor.
The Goblin fades away.
--- Round 11 over ---
...
VICTORY! Text Arena v1.0 complete. 6 of 6 monsters defeated, 0 gold, 7 items.
```

(Your waves are random: 1 monster, then 2, then 3, sorted fastest first.)
Type `run` at any time and watch every monster still in the wave fade away
as the program ends.

**How hard is it?** The first two waves are a warm-up. Wave 3 is the real
test: three monsters at once, like lesson 15. We played Arena v1.0 hundreds
of times with a script. Typing only `attack` won about 1 game in 4.
Drinking a potion whenever someone dropped to about 15-20 HP won a bit
under half. Drinking too early (when someone is only slightly hurt) did no
better than only attacking. Potions are refilled after every wave, so in
waves 1 and 2 you can afford to drink one whenever someone gets low.

## What just happened

Nobody wrote `delete`. Every monster was created by `make_unique`, owned
by exactly one `unique_ptr` inside a vector, and destroyed exactly when its
owner went away: when `erase_if` removed it, or when the whole wave went
out of scope. Whatever path the game took, no monster leaked.

Look at the order when a Goblin dies: first "The Goblin's dagger clatters to
the floor." (the Goblin's own destructor), then "The Goblin fades away."
(Monster's destructor). A child is cleaned up first, then its parent part.

That only works because `Monster.h` says `virtual ~Monster();`. The vector
deletes Goblins through a `Monster` pointer, so the destructor must be
`virtual`, for the same reason `takeTurn` is (lesson 15): otherwise only
the Monster part's destructor runs. Try it yourself 3 shows what happens
then. Rule: **a class you use through base pointers gets a virtual
destructor.**

## Break it

Open `break-it.cpp`. It tries `auto copy = wave[0];`: a second owner for
one goblin. Build it in the lesson folder. With g++ on Linux (trimmed):

```
break-it.cpp: In function 'int main()':
break-it.cpp:22:23: error: use of deleted function 'std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = Monster; _Dp = std::default_delete<Monster>]'
   22 |     auto copy = wave[0];   // two owners of one goblin?
      |                       ^
/usr/include/c++/12/bits/unique_ptr.h:514:7: note: declared here
  514 |       unique_ptr(const unique_ptr&) = delete;
      |       ^~~~~~~~~~
```

Long! Use the rule from lesson 05: find the line that names **your** file.
`break-it.cpp:22`: "use of deleted function ... `unique_ptr(const
unique_ptr&)`". That function is the *copy*, and the `note` shows it is
written `= delete`: the authors of `unique_ptr` switched copying off on
purpose. On Windows the message says it is "attempting to reference a
deleted function", with a `C` code.

Two fixes, depending on what you meant:

- **"I just want to look at it":** `auto& copy = wave[0];` (a reference,
  lesson 10). Nothing is copied.
- **"I want to take it over":** `auto copy = std::move(wave[0]);`. Now
  `copy` owns the goblin and `wave[0]` is empty. To see that, replace the
  `std::cout << copy->getName() ...` line with these:

  ```cpp
  if (!wave[0]) {
      std::cout << "wave[0] is now empty (nullptr)\n";
  }
  std::cout << copy->getName() << "\n";
  ```

  The program then prints:

  ```
  wave[0] is now empty (nullptr)
  Goblin
  The Goblin fades away.
  ```

Lesson: **there can be only one owner.** The compiler enforces it.

## Try it yourself

1. Make wave 3 always end with a boss: after `makeWave`, `push_back` one
   extra `std::make_unique<Troll>()`. (It makes the game much harder. Can
   you still win?)
2. Add a `Spawner::spawnBoss()` that returns a `std::unique_ptr<Monster>`
   holding a Troll, and print `spawner.getTotalSpawned()` at the end.
3. Remove `virtual` from `~Monster` in `Monster.h` and rebuild. The
   compiler refuses: `'Goblin::~Goblin()' marked 'override', but does not
   override`. There is no virtual destructor left to override: that is
   `override` protecting you again. Now also remove `override` from
   `~Goblin`. It compiles, with no warning. Play until a Goblin dies: "The
   Goblin's dagger clatters to the floor." never prints. The Goblin was
   deleted as if it were a plain Monster. Officially that is undefined
   behavior; here it "only" skipped the Goblin's cleanup. Put both words
   back.
4. **Part 1 checkpoint (no peeking, no grade).** You finished Part 1.
   Before Unreal, try this **without help**: add one new kind of monster
   (its own class, its own `takeTurn`, spawned by the `Spawner`) and one
   new command. If that feels hard, that is useful information, not
   failure: redo lessons 10 and 13, then try again.

## Going further

`std::shared_ptr` allows **several** owners: the object dies when the last
one lets go. It is needed less often than people think, because most things
have one clear owner. Unreal has `TUniquePtr` and `TSharedPtr` for plain
C++ objects; its game objects (`UObject`s) use a garbage collector instead
(lesson 28).

**Checkpoint: you can now** create objects at runtime with
`std::make_unique`, keep them in a vector of owners, hand ownership over
with `std::move`, and explain why a base class needs a virtual destructor.

Optional detour: [appendix-cmake](../appendix-cmake/) builds the Arena with
CMake.

Next: [19-unreal-hello](../19-unreal-hello/) - Part 2 begins: your C++ runs
inside Unreal Engine.
