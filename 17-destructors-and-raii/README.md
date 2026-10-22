# 17 - Cleanup that always happens

## Goal

Print "Round N begins" and "Round N over" around every round, and make it
impossible for "over" to be forgotten, however the round ends.

## The new idea

- **A destructor runs when an object dies.** It is written `~ClassName()`
  (a tilde, then the class name), takes nothing, returns nothing, and you
  never call it yourself. C++ calls it.
- **A local object dies when the program leaves its `{ }` block.** That
  happens at the closing brace, but also at `return`, `break` and
  `continue`. In every one of those cases, the destructor runs.
- **So: setup in the constructor, cleanup in the destructor.** Then cleanup
  cannot be forgotten. This pattern has a famous, clumsy name, **RAII**
  ("Resource Acquisition Is Initialization"). Forget the name; remember the
  idea.
- **Python bridge:** you already know RAII from Python's
  `with open("save.txt") as f:`. When the `with` block ends, for any reason,
  the file closes itself. A C++ object with a destructor works the same
  way, with no `with` keyword needed: the `{ }` block is the `with`.
- **JS bridge:** it is also like `try { ... } finally { ... }`, except the
  `finally` is attached to an object. You cannot forget to write it,
  because creating the object *is* writing it.

Think of a library book with a magic bookmark: the moment you walk out of
the room, for any reason, through any door, the book returns itself to the
shelf.

## Try it

### What changes in your Arena

Only `main.cpp` changes: a new `RoundBanner` class above `main`; a
`roundCount`; a new `run` command; and the command chain is split in two
(free commands first, then the real round, wrapped in a banner). The other
files are the same as lesson 16.

### The banner

Type the `RoundBanner` class into `my-arena/main.cpp`, above `main`:

```cpp
class RoundBanner {
public:
    RoundBanner(int roundNumber) {
        number = roundNumber;
        std::cout << "--- Round " << number << " begins ---\n";
    }

    ~RoundBanner() {
        std::cout << "--- Round " << number << " over ---\n";
    }

private:
    int number = 0;
};
```

Then, in the game loop, after the free commands (`look`, `inventory`,
`sell`) and before the real action, create one:

```cpp
++roundCount;
RoundBanner banner(roundCount);

if (verb == "run") {
    std::cout << "The party runs for the exit!\n";
    return 0;   // leave main right now
}
// ... attack or potion, then the monsters' turn ...
}   // <- end of the loop body: the banner dies here
```

That is why the old `if / else if` chain is split in two: free commands
first (each ends in `continue`), then the real round. The new `run`
command ends the game early.

Build with all four `.cpp` files, as in lesson 16:

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
Gold 0  Potions 3  > attack
--- Round 1 begins ---
Ayla hits Goblin for 10!
...
--- Round 1 over ---
...
Gold 0  Potions 3  > run
--- Round 3 begins ---
The party runs for the exit!
--- Round 3 over ---
```

## What just happened

Look at the last three lines. `return 0;` left `main` in the middle of the
round, and "Round 3 over" **still printed**. You never wrote any code to
close the round on the `run` path; nobody had to remember. Leaving the
block destroyed `banner`, and its destructor did the job.

The same happens when the last monster falls or the party is defeated: the
loop body ends, the banner closes, and only then is the result printed.

## Break it

Open `break-it.cpp`. It makes the banner the old-fashioned way, with `new`,
and never deletes it. Build it in the lesson folder **without** the
sanitizer flags, and run it. With g++ on Linux:

```
break-it.cpp: In function 'void playRound()':
break-it.cpp:23:18: warning: unused variable 'banner' [-Wunused-variable]
   23 |     RoundBanner* banner = new RoundBanner(3);
      |                  ^~~~~~
```

```
--- Round 3 begins ---
Ayla attacks the goblin.
Back in main. The round should be over by now...
```

"Round 3 over" **never** prints. Local variables live in a part of memory
called the *stack*, and they die with their block. `new` builds an object
somewhere else, on the *heap*, and gives you only a pointer to it. When
`playRound` ends, the *pointer* dies, but the banner it points at does not.
Nobody ever destroys it. That forgotten object is called a **memory leak**.
Notice the warning, too: the compiler saw a variable you never used.

Now build it with the usual sanitizer flags. On Linux, the sanitizer adds a
report when the program ends (trimmed, folder paths shortened):

```
==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 4 byte(s) in 1 object(s) allocated from:
    #0 0xffff88d1c170 in operator new(unsigned long)
    #1 0xaaaac15b17e8 in playRound() break-it.cpp:23
```

That is this leak, caught, with the exact line of the `new`. (The 4 bytes
are the banner's one `int`.) The Windows and macOS sanitizers may not
report leaks by default, so do not worry if you see nothing there.

You could fix it by writing `delete banner;` at the end of `playRound`.
But then every `return` added later needs a `delete` too, and one day
someone forgets. That is the old style, and you will see it in older code.
The better fix is the Arena's way: no `new` at all, just
`RoundBanner banner(3);`, and the destructor runs on its own. Try both.
(Notice the round brackets: `banner(3)` builds an object, just like
`banner{3}` would. Lesson 08 recommended `{ }`; you will see both styles in
real code.)

Lesson: **objects made with raw `new` are not cleaned up for you.** Lesson
18 shows the modern way to create objects at runtime without leaks.

## Try it yourself

1. Add a `RoundBanner` inside the `look` command. When does it close?
2. Make the destructor also print how many rounds have been played so far.
   (Hint: a `static int` counter inside the class, or pass a reference in.)
3. Create two banners in a row in one block, `RoundBanner a(1);` then
   `RoundBanner b(2);`. Which one closes first?

## Going further

Objects in a `std::vector` are destroyed when the vector is. Objects in
one block are destroyed in **reverse** order of creation: last made, first
gone, like a stack of plates. RAII is everywhere in C++: a `std::vector`
frees its memory in its destructor, and a file closes itself the same
way.

**Checkpoint: you can now** write a destructor, explain when a local object
dies, and use RAII so cleanup can never be forgotten.

Next: [18-unique-ptr](../18-unique-ptr/) - who owns this? Arena v1.0.
