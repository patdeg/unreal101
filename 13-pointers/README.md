# 13 - Pointers, honestly

## Goal

Give each monster a target: the party member it is after, or nobody.

**This is the hard one.** Pointers confuse almost everyone the first time,
including people who now write C++ for a living. If this lesson feels
slippery, that is normal, not a sign you are bad at this. Plan to do it
twice: once today, and again in a few days. The second time is when it
clicks for most people. Give it up to an hour, and take a break after the
warm-up if you need one.

## The new idea

- **Every object lives somewhere in memory, at an address.** An address is
  just a number, like a house number on a very long street.
- **A pointer is a variable that holds an address.** `Fighter* target` reads
  "target holds the address of a Fighter". It is like a sticky note with a
  house number written on it. The note is not the house.
- **Three symbols to learn:**

  | Code | Read it as | Sticky-note version |
  |---|---|---|
  | `&ayla` | "the address of ayla" | write Ayla's house number on a note |
  | `*target` | "the thing target points at" | walk to the house on the note |
  | `target->getName()` | same as `(*target).getName()` | walk there, ask for a name |

- **`nullptr` means "points at nobody".** A blank sticky note. Following
  it is undefined behavior (lesson 05); on a PC it almost always crashes
  the program. So you **always check first**: `if (target) { ... }`.

  ```
     the sticky note                 the street (memory)
     +--------------+
     | target       |      house 100     house 104     house 108
     | 104 ---------------------------->  [ Ayla ]      [ Bram ]
     +--------------+
       *target   = walk to house 104, and you find Ayla herself
       nullptr   = a blank note: there is no house to walk to
  ```
- **Why not a reference (lesson 10)?** A reference must always refer to
  someone, and can never be re-pointed. A monster's target changes, and is
  sometimes nobody. That is exactly what a pointer is for.
- **JS bridge:** in JavaScript, every object variable is secretly a pointer,
  and a garbage collector makes sure the object stays alive while anyone
  points at it. `null` is like `nullptr`. In C++ **nobody** guards the
  object. That is the part that bites.

## Try it

**Step 1: the warm-up.** Build and run `pointers-basics.cpp` (same commands
as always, with that file name):

```
hp is 30
hp lives at address 0xfffff49eefd0
following p gives 30
after *p = 10, hp is 10
p is empty (nullptr), so we must not follow it
```

(Your address will be a different number. That is fine.) Look at the
fourth line: we never wrote `hp = 10`, yet `hp` changed. `*p = 10` walked
to the address on the note and wrote there. Now step through it in the
debugger from lesson 12 and watch `p` and `hp` in the Locals window.

**Step 2: the Arena.** What changes in your Arena: `Fighter` gets a
`target` pointer field with `getTarget` and `setTarget`; `pickVictim`
becomes `pickTarget` and returns a pointer; the monster's turn picks a
target only when it has none; `look` says who the monster is after; and
lesson 12's crushing-blow bug is fixed. Add these to your
`my-arena/main.cpp`. The heart of it:

```cpp
Fighter* target = nullptr;            // inside the class: nobody yet

Fighter* pickTarget(std::vector<Fighter>& party) {
    if (!anyAlive(party)) {
        return nullptr;               // "nobody"
    }
    // ... pick a random living index ...
    return &party[index];             // "the address of that member"
}
```

On the monster's turn: if it has no target, or the target fell, pick one.
Then, **only if there is one**, hit it:

```cpp
if (target) {
    strike(monster, *target);
    if (!target->isAlive()) {
        monster.setTarget(nullptr);
    }
}
```

Build and run:

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

```
Gold 0  Potions 3  > attack
Ayla hits Troll for 9!
Bram hits Troll for 8!
Troll turns toward Ayla.
Troll hits Ayla for 8!
Troll raises its guard.

  Ayla HP 32/40 [Normal]
  Bram HP 50/50 [Normal]
  vs
  Troll HP 53/70 [Defending]
Gold 0  Potions 3  > look
The Troll has 53 HP left and is after Ayla.
```

## What just happened

The monster remembers *who* it is after, without copying that fighter.
`target` holds Ayla's address inside `party`. Hitting `*target` hits the
real Ayla. When she falls, the note goes back to blank (`nullptr`), and on
its next turn the monster picks again.

`&` now has two jobs, and that confuses everyone: after a **type**
(`Fighter&`) it means "reference"; in front of a **value** (`&party[i]`) it
means "address of". Same with `*`: after a type (`Fighter*`) it means
"pointer"; in front of a pointer (`*target`) it means "follow it".

## Break it

Open `break-it.cpp`. It saves a pointer to Ayla, then a new recruit joins
the vector, then it uses the pointer again.

Build it in the lesson folder **without** the sanitizer first, and run it.
On our Linux machine:

```
Captain HP: 40
Captain HP: 40
```

Looks fine! That is the scary part. Now build it **with** the sanitizer. With
g++ on Linux (trimmed, folder paths shortened):

```
Captain HP: 40
==ERROR: AddressSanitizer: heap-use-after-free on address 0xffffb4f00f70
READ of size 4 at 0xffffb4f00f70 thread T0
    #0 0xaaaad75a0684 in main break-it.cpp:20

freed by thread T0 here:
    ...
    #7 0xaaaad75a28f0 in std::vector<Fighter, std::allocator<Fighter> >::push_back(Fighter&&)
    #8 0xaaaad75a0460 in main break-it.cpp:18
```

Read it as a story: line 20 **read** memory that was already **freed**,
and it was freed by `push_back` on line 18.

Here is what happened. A vector keeps its fighters side by side in one
block of memory. When you `push_back` and the block is full, the vector
moves the whole team to a **bigger house** and knocks down the old one:

```
  before push_back:   house 100: [ Ayla ]            captain = 100
  after push_back:    house 100: (knocked down)      captain = 100  <- still!
                      house 200: [ Ayla ][ Bram ]
```

Your sticky note still has the old house number. Without the sanitizer,
you walked to an empty lot that happened to still look like Ayla's house.
That is undefined behavior again: next time it might be someone else's
house, or a crash.

The name for this is a **dangling pointer**. Lesson: **a pointer is only as
good as the object it points at.** The Arena is safe because `party` never
grows during a fight. If you add "recruit" later, remember this page.

## Try it yourself

1. Print `monster.getTarget()` (an address) next to `&party[0]` and
   `&party[1]`. Which one matches?
2. Give the **party** a target too: an `aim` command that stores a pointer
   to the monster in `party[0]`. Where is it safe to use it, and when must it
   go back to `nullptr`?
3. **Checkpoint challenge (no peeking).** Write a tiny program, no Arena:
   two `int` variables, `gold` and `gems`, and one `int* best` that points at
   whichever is bigger. Print the value through the pointer, change it
   through the pointer, and print both variables to prove the right one
   changed. Then set `best = nullptr` and make sure nothing follows it.
4. Do this lesson again in a few days. Really.

## Going further

Delete the `if (target)` check around `strike` and make `pickTarget` always
return `nullptr`. Run it in the debugger: it stops at the crash, and the
call stack shows how you got there. Following a `nullptr` is one of the
most common crashes in Unreal games, which is why Unreal code is full of
`if (OtherActor)` checks (lesson 24).

**Checkpoint: you can now** take an address with `&`, follow a pointer with
`*` and `->`, use `nullptr` for "nobody", and explain a dangling pointer.

Next: [14-header-and-source](../14-header-and-source/) - split the Arena into
several files.
