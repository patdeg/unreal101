# 03 - Decisions and loops

## Goal

Make the hero and a goblin fight, turn by turn, until one falls.

## The new idea

- **`if`, `else` and `while` look just like JavaScript.** Conditions go in
  `( )`, blocks in `{ }`. Use `&&` for "and" and `||` for "or".
- **C++ has `==` but no `===`.** When the two sides have different types,
  C++ either converts one side by fixed rules (`1 == 1.0` is true) or
  refuses to compile. There is no "loose or strict" choice to make.
- **Reading from the keyboard:** `std::cin >> choice;` is the opposite of
  `std::cout`: it reads what the player types into a variable. (Browser
  JavaScript has `prompt()`; Python has `input()`.)
- **`char` holds one character.** `'a'` (single quotes) is one character.
  `"a"` (double quotes) is text. In C++ they are different types.

## Try it

In `my-arena`, replace `main.cpp` with this lesson's program (type it). The
fight loop:

```cpp
while (heroHp > 0 && monsterHp > 0) {
    // show HP, ask for a move
    // attack, or heal, or lose the turn
    // the goblin hits back if it is alive
}
```

Build and run (commands as in lesson 01). A sample game:

```
Ayla faces a Goblin!

Ayla HP 40   Goblin HP 30
(a)ttack or (h)eal? a
Ayla hits for 12!
Goblin hits for 7!
...
Victory! Ayla wins.
```

Type **one letter**, then Enter: `a` to attack, `h` to heal. To stop early,
press Ctrl+C. If you type a whole word such as `heal`, watch what happens:
the program reads it one letter at a time, `h`, `e`, `a`, `l`, and plays
four turns. Lesson 06 fixes that.

## What just happened

The `while` loop repeats as long as both fighters are alive. Each time
round, the program reads one character, and an `if / else if / else` chain
picks what happens. `-=` and `+=` are shortcuts: `monsterHp -= 12;` means
`monsterHp = monsterHp - 12;`.

One line needs a closer look: `if (!(std::cin >> choice))`. Reading with
`std::cin >> choice` also answers a question: did the read work? It is true
when a character arrived, and false when the input has ended (you pressed
Ctrl+Z then Enter on Windows, or Ctrl+D elsewhere). `!` flips it, so the
`if` runs when there is nothing left to read, and the game says goodbye
instead of looping forever.

## Break it

Open `break-it.cpp` (build it in the lesson folder, as in lesson 01). Spot
the difference from a normal `if`? It has **one** equals sign:
`if (choice = 'a')`.

It compiles. But our build commands turn warnings on (`/W4` on Windows,
`-Wall` on g++), and g++ says:

```
break-it.cpp:10:16: warning: suggest parentheses around assignment used as truth value [-Wparentheses]
   10 |     if (choice = 'a') {
      |         ~~~~~~~^~~~~
```

Now run it and press `h`. It says "You attack!" anyway. A single `=` does
not compare: it **puts** `'a'` into `choice`, and the result counts as true.
Every key attacks.

Lesson: some bugs compile fine. **Warnings are an early alarm. Read them.**
Fix it with `==`, rebuild, and press `h` again.

## Try it yourself

1. Add a `(r)un` option that ends the game.
2. Give the hero 3 potions and stop healing when they run out. (You need a new
   `int` variable.)
3. Make the goblin heal sometimes: when its HP is below 10, it heals instead
   of attacking.

## Going further

A `for` loop is the neat way to repeat a known number of times:
`for (int i = 0; i < 3; i++) { ... }`. Same as JS, except `i` has a type.
`break` leaves a loop; `continue` skips to the next round.

**Checkpoint: you can now** make decisions with `if`, repeat with `while`,
and read what the player types.

Next: [04-functions](../04-functions/) - roll dice for random damage.
