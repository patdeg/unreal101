# 08 - Your own types

## Goal

Describe the hero and every monster with one type of your own: `Fighter`.

## The new idea

- **A `struct` is a new type you invent.** It bundles values that belong
  together:

  ```cpp
  struct Fighter {
      std::string name;
      int hp = 0;
      int minDamage = 0;
      int maxDamage = 0;
  };
  ```

- **It is like a JavaScript object with a fixed shape.** In JS,
  `{ name: "Goblin", hp: 30 }` can grow new fields at any time, and a
  misspelled field is just `undefined`. A `Fighter` has exactly these four
  fields, forever, and the compiler checks every one you use.
- **Making one:** `Fighter goblin{"Goblin", 30, 4, 9};` fills the fields in
  order. Reading one: `goblin.hp`, with a dot, just like JS.
- **About those curly braces.** `{ }` right after a name builds an object
  from the values inside. You already typed it in lesson 04
  (`generator{...}`), and you will also see `( )` doing the same job
  (lesson 17). Prefer `{ }`: it refuses conversions that lose data.
  `int x{2.5};` is an error, while `int x = 2.5;` quietly drops the `.5`.

## Try it

Type the `struct` above `rollDice` in your `my-arena/main.cpp` (do not
forget the `;` after the closing brace). Then replace the loose variables:

```cpp
Fighter hero{"Ayla", 60, 8, 16};

const std::vector<Fighter> monsters{
    {"Goblin", 30, 4, 9},
    {"Wolf", 30, 5, 10},
    {"Goblin Chief", 50, 6, 12},
};
```

Each monster now has its own stats, and they are tougher than before.

Everywhere the old code said `heroHp`, it now says `hero.hp`; `monsterName`
becomes `monster.name`. Damage comes from each fighter's own range:

```cpp
int damage = rollDice(hero.minDamage, hero.maxDamage);
monster.hp -= damage;
```

One game change comes with this lesson: **potions are limited**. Until
now, `use potion` worked forever, so you could never lose. Now the hero
carries 3, each heals 15, and the status line shows how many are left. It
uses only things you already know:

```cpp
int potions = 3;
const int potionHeal = 15;

// in the command chain:
} else if (verb == "use" && object == "potion") {
    if (potions == 0) {
        std::cout << "You have no potions left!\n";
        continue;   // no potion, no turn used
    }
    potions -= 1;
    hero.hp += potionHeal;
    // ...
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

`look` now tells you how hard the monster hits, and the Goblin Chief really
is tougher:

```
A Goblin Chief appears!

Ayla HP 35  Gold 0  Potions 2   Goblin Chief HP 50
> look
A Goblin Chief with 50 HP. It hits for 6-12.

Ayla HP 35  Gold 0  Potions 2   Goblin Chief HP 50
> attack
Ayla hits for 10!
Goblin Chief hits for 12!
```

The game is now winnable, but not free. If you only ever type `attack`,
you lose a bit more than half the time (we played it hundreds of times with
a script to check). Drinking a potion whenever the hero drops to about 15
HP wins almost every game. Save them for the Goblin Chief if you can.

## What just happened

You made the program's ideas match the game's ideas. Before, "the hero" was
four separate variables that happened to have similar names. Now it is one
thing, `hero`, that you can store in a vector, pass around, and print.

Look at the loop: `for (Fighter monster : monsters)`. There is no `&`, so
each pass makes a **fresh copy** of the monster. The fight lowers the
copy's `hp`, and the list stays untouched. Here that is exactly what we
want. In lesson 10 a copy causes a bug, and you learn to choose on
purpose.

## Break it

Open `break-it.cpp`. It asks for `goblin.health`, but the field is called
`hp`. Build it in the lesson folder. With g++ on Linux:

```
break-it.cpp: In function 'int main()':
break-it.cpp:13:51: error: 'struct Fighter' has no member named 'health'
   13 |     std::cout << goblin.name << " has " << goblin.health << " HP\n";
      |                                                   ^~~~~~
```

On Windows the wording differs and you get an error code starting with `C`,
but it says the same thing: `Fighter` has no `health`.

In JavaScript, `goblin.health` would quietly be `undefined`, and the bug
would show up later as `"Goblin has undefined HP"` (or worse, as `NaN`
damage in the middle of a fight). In C++ it never gets that far. Lesson: **a
fixed shape turns a typo into a compile error.**

## Try it yourself

1. Add a fifth field, `int speed = 0;`, and give every fighter a speed. Show
   it in `look`. (Speed comes back in lesson 15.)
2. Add a fourth monster to the list. One line!
3. Write a function `void printFighter(Fighter f)` that prints a stat card,
   and call it for the hero at the start of the game.

## Going further

You can name the fields when you make a fighter:
`Fighter goblin{.name = "Goblin", .hp = 30, .minDamage = 4, .maxDamage = 9};`
(C++20 "designated initializers"). The names must be in the same order as
in the struct. Unreal's version of a plain data bundle is a `USTRUCT`
(lesson 27).

**Checkpoint: you can now** invent your own type with a `struct`, make
values of it with `{ }`, and keep them in a vector.

Next: [09-enums-and-switch](../09-enums-and-switch/) - monsters learn to
defend, and sometimes get stunned.
