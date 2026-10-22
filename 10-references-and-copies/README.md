# 10 - Copies vs references

## Goal

Recruit a second fighter, and make "heal the whole party" actually heal.

## The new idea

In this lesson your JavaScript instincts are **wrong**, so go slowly. It is
not you: this trips up everyone who comes from JS.

- **In JavaScript, objects are shared.** `let b = a;` gives you two names for
  the *same* object. Change `b.hp` and `a.hp` changes too. (Python works the
  same way: `b = a` shares one object.)
- **In C++, `=` makes a copy.** `Fighter b = a;` builds a brand new fighter
  with the same values. Change `b.hp` and `a` does not notice.
- **`&` makes a reference: another name for the same thing.**
  `Fighter& b = a;` means "`b` *is* `a`, under a second name". Change `b.hp`
  and `a.hp` changes, because there is only one fighter.

Think of a character sheet in a tabletop game:

| C++ | Analogy |
|---|---|
| `Fighter f` | A **photocopy** of the sheet. Scribble on it all you like; the original is untouched. |
| `Fighter& f` | A **nickname**. "Ayla" and "the captain" are the same person. Hurt one, you hurt both. |
| `const Fighter& f` | The original sheet **behind glass**: you can read it, but not write on it. No copy is made. |

The same rule applies to function parameters and to range-for loops. That
is where it bites.

## Try it

Add the new parts to your `my-arena/main.cpp`. The party is a vector of
fighters:

```cpp
std::vector<Fighter> party{
    {"Ayla", 40, 8, 16},
    {"Bram", 50, 5, 10},
};
```

Then three small functions, each choosing on purpose:

```cpp
void printStatus(const Fighter& fighter);              // look only
void strike(const Fighter& attacker, Fighter& defender); // hurt the defender
void healParty(std::vector<Fighter>& party, int amount); // change the party
```

Inside `healParty`, the loop says `auto&`, so each `member` is the real
party member, not a copy:

```cpp
for (auto& member : party) {
    if (member.hp > 0) {
        member.hp += amount;
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

Now `attack` makes both fighters strike, the monster picks a random
victim, and one potion heals everyone still standing by 20. Two heroes hit
much harder than one, so the monsters got tougher to match (the Goblin
Chief now has 90 HP):

```
  Ayla HP 29 [Normal]
  Bram HP 50 [Normal]
  vs
  Goblin HP 28 [Defending]
Gold 0  Potions 3  > use potion
The party shares a potion (+20 HP each).
Goblin hits Ayla for 8!

  Ayla HP 41 [Normal]
  Bram HP 70 [Normal]
  vs
  Goblin HP 28 [Normal]
```

Look at Bram: 70 HP, more than he started with. Nothing stops it yet.
Lesson 11 fixes that.

## What just happened

Every place that takes a fighter now says what it plans to do with it:

- `printStatus(const Fighter&)`: only reads. No copy, no changes.
- `strike(const Fighter& attacker, Fighter& defender)`: reads the attacker,
  hurts the **real** defender.
- `Fighter& pickVictim(...)` even **returns** a reference: "here is the real
  party member", so the hit lands on the party, not on a copy.

## Break it

Open `break-it.cpp`. It heals a party in a loop. Build and run it in the
lesson folder. With g++ on Linux:

```
Before: Ayla 20, Bram 15
  healing Ayla to 30
  healing Bram to 25
After:  Ayla 20, Bram 15
```

Read that carefully. Inside the loop, the healing *worked*: 20 became 30.
But afterwards, nobody was healed. No error, no warning.

Here is why. `for (auto member : party)` has no `&`, so on each pass C++
**photocopies** one fighter into `member`. You healed the photocopy. At the
end of the pass, the photocopy was thrown away. (In JavaScript, this exact
loop would have worked, which is why this bug is so common.)

Fix it by adding one character: `for (auto& member : party)`. Rebuild:

```
After:  Ayla 30, Bram 25
```

Now the other direction. In your `my-arena/main.cpp`, add
`fighter.hp = 0;` as the first line of `printStatus`, and build. With g++
(your line number may differ):

```
main.cpp: In function 'void printStatus(const Fighter&)':
main.cpp:55:16: error: assignment of member 'Fighter::hp' in read-only object
   55 |     fighter.hp = 0;
      |     ~~~~~~~~~~~^~~
```

`const` kept its promise: a function that is supposed to *look* cannot
change anything. (Windows says the same with a `C` error code.) Remove the
line again.

Lesson: **decide on purpose. Copy, reference, or const reference.**

## Try it yourself

1. Add a third party member, `"Cleo"`. Nothing else needs to change. Why?
2. Write `void reviveParty(std::vector<Fighter>& party)` that sets every
   fallen member back to 10 HP, and add a `revive` command. Try it once
   without the `&` and watch it do nothing.
3. Change `printStatus` to take `Fighter` (a copy). It still works. Why is
   `const Fighter&` still the better choice? (Hint: what does a copy cost?)

## Going further

Never return a reference to a variable that lives inside the function. When
the function ends, that variable is gone, and the reference points at
nothing. g++ warns:

```
warning: reference to local variable 'goblin' returned [-Wreturn-local-addr]
```

Unreal functions take `const FHitResult& Hit` for exactly the reason in this
lesson: look, don't copy.

**Checkpoint: you can now** choose between a copy, a reference and a const
reference, and explain why a loop without `&` changes nothing.

Next: [11-classes](../11-classes/) - make it impossible for HP to go below
zero.
