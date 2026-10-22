# 15 - One interface, many monsters

## Goal

Fill the arena with a Goblin, a Troll and a Slime at once, each fighting in
its own way, while the game treats them all the same.

This is one of the three hard lessons (with 13 and 18), and it can take
closer to an hour. The new idea has two parts, **A** (inheritance) and
**B** (`virtual`), and each makes sense alone. Stopping between them, even
for a day, is a good idea.

## The new idea

### Part A: inheritance

- **Inheritance means "is a".** `class Monster : public Fighter` means a
  Monster *is a* Fighter, plus more. It gets everything a Fighter has (name,
  HP, `takeDamage`, ...) for free, and adds a `speed`.
  `class Goblin : public Monster` means a Goblin is a Monster. **JS
  bridge:** `class Goblin extends Monster`.
- **A child builds its parent first.** A Monster's constructor must hand the
  name, HP and damage up to Fighter's constructor. It does that with an
  **initializer list**, the part after the colon:

  ```cpp
  Monster::Monster(const std::string& startName, int startHp, int minDmg,
                   int maxDmg, int startSpeed)
      : Fighter(startName, startHp, minDmg, maxDmg) {   // build the Fighter part
      speed = startSpeed;                               // then our own field
  }
  ```

  Read `: Fighter(...)` as "first, build me as a Fighter with these". JS
  writes the same idea as `super(name, hp, ...)` on the first line of the
  constructor. (Lesson 11's "Going further" showed the initializer list for
  plain fields; here it is required, because Fighter has no constructor
  without arguments.) The Goblin's constructor does the same, handing
  its stats up to Monster:
  `Goblin::Goblin() : Monster("Goblin", 30, 2, 6, 8) {}`.

### Part B: `virtual`

- **`virtual` lets the real object decide.** The parent says
  `virtual void takeTurn(...)`, and each child writes its own version marked
  `override`.
- **A parent pointer can point at a child.** A `Monster*` can hold the
  address of a Goblin, because a Goblin is a Monster. Call
  `monster->takeTurn(party)` and the **Goblin's** version runs. The game loop
  never needs to know which kind of monster it holds.
- **The difference from JavaScript:** in JS, the object's own method always
  runs. In C++ that is only true for `virtual` functions. A child *can*
  write its own non-virtual function with the same name, but then the
  **pointer's** type picks the version: through a `Monster*` you get the
  Monster version, silently. That is today's "Break it".

## Try it

### What changes in your Arena

- **New files `Monster.h` and `Monster.cpp`:** the `Monster` class and three
  children, `Goblin` (fast, stabs twice), `Troll` (regenerates) and `Slime`
  (splits once).
- **`Fighter.h` / `Fighter.cpp`:** `printStatus`, `anyAlive` and `strike`
  move here from `main.cpp`, so `Monster.cpp` can use them too.
  `pickTarget` moves into `Monster.cpp`.
- **`main.cpp`:** **one** big fight against all three monsters at once
  replaces the three fights in a row. The party always attacks the
  first monster still standing, then every living monster takes its turn.

Copy the new and changed files into `my-arena` (or type them while
reading). The game is new enough that copying `main.cpp` is the sensible
choice.

### The new classes

The shape of `Monster.h`:

```cpp
class Monster : public Fighter {
public:
    virtual void takeTurn(std::vector<Fighter>& party);
    // ... building blocks: recoverFromStun, attackTarget, chooseNextStance
};

class Goblin : public Monster {
public:
    Goblin();
    void takeTurn(std::vector<Fighter>& party) override;   // stabs twice
};
// ... Troll (regenerates) and Slime (splits once) look the same
```

And in `main.cpp`, three plain objects and one list of parent pointers:

```cpp
Goblin goblin;
Troll troll;
Slime slime;
std::vector<Monster*> wave{&goblin, &troll, &slime};

for (Monster* monster : wave) {
    if (monster->isAlive() && anyAlive(party)) {
        monster->takeTurn(party);    // the right version runs for each
    }
}
```

Build with **all four** `.cpp` files:

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

```
  Ayla HP 29/40 [Normal]
  Bram HP 50/50 [Normal]
  vs
  Goblin HP 6/30 [Normal]
  Troll HP 45/45 [Normal]
  Slime HP 35/35 [Normal]
Gold 0  Potions 3  > attack
CRUSHING BLOW!
Ayla hits Goblin for 21!
The Goblin falls and drops a healing herb.
Bram hits Troll for 5!
The Troll's wounds close a little (+3 HP).
Troll hits Ayla for 6!
Slime hits Ayla for 4!
Slime raises its guard.
```

**This fight is hard on purpose.** Three monsters hit you every round, so
you cannot just type `attack` and win: we played it hundreds of times with
a script, and attacking only won about 1 game in 3. Drinking a potion when
someone drops to about 15 HP won about half the games. A potion heals
everyone standing by 20, so it is worth the most when **both** heroes are
hurt, and it cannot bring back a hero who has fallen.

## What just happened

One loop, three behaviors. The loop only says `monster->takeTurn(party)`.
Because `takeTurn` is `virtual`, C++ looks at the **real** object behind the
pointer at that moment and runs its version: the Goblin stabs twice, the
Troll heals then fights, the Slime may split. Add a fourth kind of monster
later and the loop does not change at all.

Look at `Troll::takeTurn` in `Monster.cpp`: after healing, it calls
`Monster::takeTurn(party);` to do the normal fight. That is "do what my
parent does". Unreal writes the same idea as `Super::BeginPlay();`, and you
will type it in almost every Unreal class.

## Break it

Open `break-it.cpp`. The base class forgot `virtual`. Build and run it in
the lesson folder. With g++ on Linux:

```
Calling each one directly:
  The goblin stabs twice!
  The troll regenerates, then smashes!
Calling through Monster* pointers:
  The monster attacks.
  The monster attacks.
```

No error, no warning. Called directly, each child's own `takeTurn` runs.
Called through a `Monster*` pointer, every monster acts like a plain
Monster: without `virtual`, `monster->takeTurn()` looks only at the
pointer's type (`Monster*`) and calls `Monster::takeTurn`. Add `virtual` in
front of the base `takeTurn`, rebuild, and both lines turn into the right
attacks.

Now get insurance. Add `override` to the Goblin's version, and misspell
its name:

```cpp
void takeTrun() const override {
```

With g++:

```
break-it.cpp:15:10: error: 'void Goblin::takeTrun() const' marked 'override', but does not override
   15 |     void takeTrun() const override {
      |          ^~~~~~~~
```

Without `override`, that typo would have made a brand-new, never-called
function, and the goblin would quietly act like a plain monster again. With
`override`, the compiler checks. (Windows gives a similar error with a `C`
code.) Bonus: `override` also catches a missing `virtual` in the parent.

Lesson: **`virtual` in the parent, `override` in the child. Always both.**

## Try it yourself

1. Make a `Bat` class: fast (speed 9), weak, and it misses 1 time in 3. Add
   one to the wave.
2. Give the Slime a `look` message of its own. Hint: a new
   `virtual std::string describe() const` in `Monster`.
3. Remove `Troll::takeTurn` entirely (from `.h` and `.cpp`). What does the
   troll do now? Why does that make sense?

## Going further

- `protected:` is a third door, between `public` and `private`: children
  may use it, outsiders may not.
- In `Fighter.cpp`, `static int addCrushingBonus(...)` shows a new use of
  `static`: in front of a function outside any class, it means "only this
  file can see this function". (Inside `rollDice`, lesson 04's `static`
  meant something else: "keep this alive between calls". Same word, two
  jobs.)
- Base classes used through pointers also need a `virtual` destructor;
  lesson 18 shows what goes wrong without one.
- Every Unreal actor works this way: you override `BeginPlay` and `Tick`,
  and the engine calls them through a base pointer.

**Checkpoint: you can now** build a family of classes with inheritance,
call the parent's constructor and functions, and make the right child
function run with `virtual` and `override`.

Next: [16-lambdas](../16-lambdas/) - sort the turn order with a function you
write on the spot.
