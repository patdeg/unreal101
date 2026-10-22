# 11 - Classes guard their data

## Goal

Make it impossible for any fighter's HP to go below zero, or above its
maximum.

## The new idea

- **A `class` is a struct with walls.** It bundles data *and* the functions
  that work on that data. Some parts are `public:` (anyone may use them),
  some are `private:` (only the class's own functions may touch them).
- **Only the class can change its private data.** HP becomes private, so
  the only way to lower it is `takeDamage()`, and that one function makes
  sure HP stops at 0. One rule, one place, enforced by the compiler.
- **A constructor sets up a new object.** It is a function with the class's
  name and no return type. Every `Fighter` is built through it, so none can
  be half set up.
- **JS bridge:** JavaScript's `class Fighter { #hp; constructor(...) {...} }`
  is very close. `#hp` is JS's private field; C++ writes `private:` above
  the fields instead. **Python bridge:** Python has no real private fields;
  you write `_hp` and *ask* people not to touch it. C++ makes it a rule.

## Try it

### What changes in your Arena

So you know which lines to edit in your `my-arena/main.cpp`:

- `struct Fighter` becomes `class Fighter`, with a constructor, small
  "getter" functions (`getName`, `getHp`, ...), `takeDamage` and `heal`.
  Its fields move under `private:`.
- `printStatus`, `anyAlive`, `strike`, `healParty` and `pickVictim` now call
  those functions instead of touching fields.
- In `main`: the typed name builds a new `Fighter` (the name is private
  now), `look` shows the monster's HP, and `.hp` checks become
  `isAlive()`.

Nothing else moves.

### The class

Turn the `struct Fighter` into a class. The shape (see `main.cpp` for the
whole thing):

```cpp
class Fighter {
public:
    Fighter(const std::string& startName, int startHp, int minDmg, int maxDmg) {
        name = startName;
        hp = startHp;
        maxHp = startHp;
        minDamage = minDmg;
        maxDamage = maxDmg;
    }

    int getHp() const { return hp; }
    bool isAlive() const { return hp > 0; }

    int takeDamage(int amount) {
        if (stance == Stance::Defending) { amount = amount / 2; }
        hp -= amount;
        if (hp < 0) { hp = 0; }      // the guard
        return amount;
    }

private:
    std::string name;
    int hp = 0;
    int maxHp = 0;
    // ...
};
```

Then fix every place that used to reach in: `fighter.hp` becomes
`fighter.getHp()`, `monster.hp -= damage` becomes
`monster.takeDamage(damage)`, and so on. The compiler lists every place you
missed. That list is your to-do list.

Notice the `const` after `getHp()`. It promises "this function only looks".
Lesson 10's `printStatus(const Fighter&)` can only call functions that make
that promise.

The getter for the name has even more decoration:

```cpp
const std::string& getName() const { return name; }
```

Read it in two parts. The `const std::string&` at the front means "here is
the real name, read-only, no copy" (lesson 10). The `const` at the end means
"this function only looks", as above.

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

HP now shows as current/maximum, and potions cannot overfill anyone.
Drink one at full HP and it is simply wasted:

```
  Sir Gob HP 40/40 [Normal]
  Bram HP 50/50 [Normal]
  vs
  Goblin HP 50/50 [Normal]
Gold 0  Potions 3  > use potion
The party shares a potion (+20 HP each).
Goblin hits Sir Gob for 11!
Goblin raises its guard.

  Sir Gob HP 29/40 [Normal]
  Bram HP 50/50 [Normal]
  vs
  Goblin HP 50/50 [Defending]
Gold 0  Potions 2  >
```

## What just happened

Before, any line in the program could write `hp = -50` or `hp = 9999`,
and you had to trust every line. Now the class makes a promise: **HP is
always between 0 and the maximum**. You only have to check `takeDamage` and
`heal` to know that is true, because nothing else can touch `hp`.

`strike` got simpler too. It no longer does HP math; it asks the defender to
`takeDamage`, and the defender applies its own rules, including halving
damage while defending.

## Break it

Open `break-it.cpp`. It tries `goblin.hp = -50;` from `main`. Build it in
the lesson folder. With g++ on Linux:

```
break-it.cpp: In function 'int main()':
break-it.cpp:22:12: error: 'int Fighter::hp' is private within this context
   22 |     goblin.hp = -50;   // "just set it directly, it's faster"
      |            ^~
break-it.cpp:17:9: note: declared private here
   17 |     int hp = 0;
      |         ^~
```

Two parts: the line that broke the rule, and the line that made the rule
(`declared private here`). Newer g++ versions add a third part:

```
break-it.cpp:22:12: note: field 'int Fighter::hp' can be accessed via 'int Fighter::getHp() const'
   22 |     goblin.hp = -50;   // "just set it directly, it's faster"
      |            ^~
      |            getHp()
```

It means: you may *read* the HP through `getHp()`. Do not follow the
suggestion literally: `goblin.getHp() = -50;` is another error, because you
cannot *assign* through a getter. That is the point of the class. On
Windows you get an error code starting with `C` that says it "cannot access
private member".

Lesson: **a class is a promise about what can happen to its data.** The
compiler holds everyone to it, including you, at 11 pm, in a hurry.

## Try it yourself

1. Add `void restoreFull()` that sets HP back to the maximum, and call it on
   the party after each win. (Lesson 18 does this for real, between waves.)
2. Make `heal` return how much it really healed, and print it:
   `Bram +6 (now full)`. Who decides what "really healed" means: `main`, or
   the class?
3. Try to call `takeDamage` on a `const Fighter&` (for example inside
   `printStatus`). Read the error. Why does the compiler refuse?
4. **Checkpoint challenge (no peeking).** The potion count is a loose `int`
   in `main` that any line could set to 99. Write a small class
   `PotionBag` with a private count, a constructor that takes the starting
   count, `int left() const`, and `bool drink()` that returns `false` when
   the bag is empty. Use it instead of `int potions`.

## Going further

- **A neater constructor.** The constructor above *assigns* each field in
  its body. C++ has a second way, the **initializer list**: after the `)`
  and a colon, each field is built directly from a value.

  ```cpp
  Fighter(const std::string& startName, int startHp, int minDmg, int maxDmg)
      : name{startName}, hp{startHp}, maxHp{startHp},
        minDamage{minDmg}, maxDamage{maxDmg} {}
  ```

  Both work here. Unreal constructors use the initializer-list form all the
  time, and lesson 15 needs it, so it is worth recognizing now.
- Inside a member function, `this` is a pointer to the object the
  function was called on (`this->hp` is the same as `hp`). Lesson 13
  explains pointers; after that you will see `this` all the time in Unreal
  code. Every Unreal type you meet in Part 2 is a class.

**Checkpoint: you can now** write a class with private data, a constructor,
and `const` functions that only look.

Next: [12-debugger](../12-debugger/) - a sneaky bug, and the tool that
catches it.
