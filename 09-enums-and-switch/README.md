# 09 - Named choices

## Goal

Give monsters a stance (Normal, Defending or Stunned) that changes how the
fight goes.

## The new idea

- **`enum class` names a fixed set of choices.**

  ```cpp
  enum class Stance { Normal, Defending, Stunned };
  ```

  A `Stance` can only ever be one of those three. You write them as
  `Stance::Defending`.
- **JavaScript has no enums.** People use strings: `stance = "defending"`.
  Then someone types `"defnding"`, nothing complains, and the monster never
  defends. In C++, `Stance::Defnding` is a compile error.
- **`switch` picks a block by value**, like JavaScript's `switch`. It works
  on whole-number-like things (ints, chars, enums), not on strings. Each
  `case` ends with `break` (or `return`). Forget the `break` and C++ runs
  straight on into the next case, just like JavaScript.

## Try it

Add the new parts to your `my-arena/main.cpp`: the `enum class` above the
struct, a new field in `Fighter`, and a `stanceName` function that turns a
stance into text:

```cpp
struct Fighter {
    std::string name;
    int hp = 0;
    int minDamage = 0;
    int maxDamage = 0;
    Stance stance = Stance::Normal;
};
```

Then the rules, using `switch`:

- When the hero attacks, a **Defending** monster takes half damage.
- A hit of 14 or more makes the monster **Stunned**: it loses its next turn.
- After the monster attacks, it raises its guard (Defending) for the next
  turn 1 time in 3.

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

A sample fight (the stance shows in brackets):

```
Ayla HP 19  Gold 0  Potions 3   Goblin Chief HP 41 [Defending]
> attack
The Goblin Chief blocks half the blow.
Ayla hits for 6!
Goblin Chief hits for 8!

Ayla HP 11  Gold 0  Potions 3   Goblin Chief HP 35 [Normal]
> attack
Ayla hits for 15!
The Goblin Chief staggers. Stunned!
The Goblin Chief shakes off the stun and does nothing.
```

Tip: a Defending monster only takes half damage. If you were going to
drink a potion soon anyway, that is a good turn to do it.

## What just happened

1. The `stance` field stores one of three named values. It starts as
   `Stance::Normal`, its default.
2. Each `switch (monster.stance)` jumped straight to the matching `case`.
   Two cases can share one block by stacking them:

   ```cpp
   case Stance::Normal:
   case Stance::Defending: {
       // both stances attack
       break;
   }
   ```

3. A `case` that makes its own variables needs `{ }` around its block. That
   is why the attack case has braces.

## Break it

Open `break-it.cpp`. It reasons, "Defending is the second one, so it must
be 1", and writes `if (goblinStance == 1)`. Build it in the lesson folder.
With g++ on Linux:

```
break-it.cpp: In function 'int main()':
break-it.cpp:14:22: error: no match for 'operator==' (operand types are 'Stance' and 'int')
   14 |     if (goblinStance == 1) {
      |         ~~~~~~~~~~~~ ^~ ~
      |         |               |
      |         Stance          int
```

Read the bottom picture: the left side is a `Stance`, the right side is an
`int`, and C++ will not compare them. On Windows you get a similar error
with a `C` code.

That refusal is the whole point. An `enum class` is **not** a number in
disguise. If someone later adds a new stance at the top of the list, every
"1" in the program would silently mean something else. With names, nothing
breaks. Fix it: `if (goblinStance == Stance::Defending)`.

(Old C++ has a plain `enum`, without `class`, that *does* allow `== 1`. You
will see it in older code. It is the source of exactly that kind of bug.)

## Try it yourself

1. Add a fourth stance, `Enraged`, where the monster hits for 2 extra damage.
   Add it to `stanceName` too.
2. Make a Stunned monster take double damage from the hero.
3. Give the **hero** a stance: a `defend` command that sets
   `hero.stance = Stance::Defending` and halves the next monster hit.

## Going further

Delete the `Stunned` case from `stanceName` and rebuild. g++ says:

```
warning: enumeration value 'Stunned' not handled in switch [-Wswitch]
```

That warning catches forgotten cases when you add a new stance. Whether
Visual Studio warns here depends on its settings. Unreal uses enum classes
everywhere, for example `ETriggerEvent::Started` when a key is pressed
(lesson 23).

**Checkpoint: you can now** name a fixed set of choices with `enum class`
and pick a block of code with `switch`.

Next: [10-references-and-copies](../10-references-and-copies/) - recruit a
party, and find out why healing it does nothing (at first).
