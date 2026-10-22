# 04 - Functions with types

## Goal

Replace fixed damage with random damage from a function called `rollDice`.

## The new idea

- **A function declares what goes in and what comes out.**
  `int rollDice(int min, int max)` means: give me two whole numbers, I give
  back one. In JavaScript `function roll(min, max)` accepts anything. Here the
  compiler checks every call.
- **Order matters.** The compiler reads top to bottom, so write a function
  *above* the place you call it. (Lesson 14 shows a tidier way.)
- **`return` hands the answer back.** If a function says it returns an
  `int`, every path through it must end in a `return` with an `int`.
  Careful: the compiler only *warns* if you forget one. Treat that warning
  as an error. A missing `return` hands back garbage, not a friendly
  `undefined` like in JavaScript.

## Try it

From now on the Arena grows lesson by lesson. Add the new parts to your
`my-arena/main.cpp` (this lesson's `main.cpp` shows the whole thing). The
new piece sits above `main`:

```cpp
int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}
```

Those three lines are a "dice machine". Don't worry about how they work
yet; just know that `rollDice(8, 16)` gives a random whole number from 8 to
16. (The curly braces in `generator{...}` and `random_device{}` build
objects; lesson 08 explains them.)

Now the fight uses it:

```cpp
int damage = rollDice(8, 16);
monsterHp -= damage;
```

Build and run. Every fight is now different:

```
Ayla HP 36   Goblin HP 22
(a)ttack or (h)eal? a
Ayla hits for 8!
Goblin hits for 9!
```

## What just happened

You pulled a job into one named place. To change how damage works later,
you edit one function, not many lines. Notice also that `damage` is a **new
variable inside the `if` block**; it lives only until the closing `}`. That
rule, "a variable lives inside the braces where it was made", is called
*scope*, and it matters a lot later.

## Break it

Open `break-it.cpp`. It calls `rollDice("one", 6)`: text where a number
goes. Build it. With g++ (trimmed):

```
break-it.cpp:9:27: error: invalid conversion from 'const char*' to 'int' [-fpermissive]
    9 |     std::cout << rollDice("one", 6) << "\n";
      |                           ^~~~~
break-it.cpp:4:18: note:   initializing argument 1 of 'int rollDice(int, int)'
    4 | int rollDice(int min, int max) {
      |              ~~~~^~~
```

(As in lesson 02, ignore `[-fpermissive]`, and never use it.)

The compiler points at your call, then at the function's first line: its
contract. In JavaScript this would run and produce nonsense. Here it is
stopped before the program even exists.

## Try it yourself

1. Add a "critical hit": roll `rollDice(1, 20)`, and if it comes up 20,
   double the damage.
2. Write a second function `void printHp(int hp)` that prints an HP line
   (`void` means "returns nothing"), and call it from the loop.
3. Turn `return distribution(generator);` into just
   `distribution(generator);` and build. The compiler only warns (g++:
   `no return statement in function returning non-void`). Run the game
   anyway. On our Linux machine the normal build printed a damage number
   but ended the fight after one hit, and an optimized build (add `-O2`)
   froze the moment the hero attacked. Nobody can say what it will do. That
   is why a missing-`return` warning must be treated as an error.

## Going further

You can have two functions with the same name but different inputs
(`rollDice(int sides)` and `rollDice(int min, int max)`). This is called
*overloading*. Unreal uses it a lot.

**Checkpoint: you can now** write a function with typed inputs and a typed
result, and call it.

Next: [05-vectors](../05-vectors/) - give the hero an inventory.
