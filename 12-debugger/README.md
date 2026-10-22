# 12 - Watching your program think

## Goal

Find a bug by pausing the running program and looking inside, instead of
guessing.

## The new idea

- **A debugger runs your program one step at a time.** You pick a line (a
  **breakpoint**), the program freezes when it gets there, and you can read
  every variable, then step forward line by line.
- **JS bridge:** this is the browser DevTools debugger, or the `debugger;`
  statement, for C++. Same buttons: continue, step over, step into.
- **The call stack answers "who called me?"** When the program is paused
  inside a function, the call stack lists the chain of calls that led there,
  newest on top.
- **Debug info is part of the build.** `/Zi` (Windows) and `-g`
  (Linux/macOS) save a map from machine code back to your lines. Without
  it, the debugger is lost. You have used these flags since lesson 05.

## Try it

(This lesson can take an hour. The warm-up and the hunt make two good
sittings.)

### The bug report

This lesson's `main.cpp` brings in a Troll (it replaces the Goblin Chief
and fights first) and a new rule: a perfect damage roll, the attacker's
maximum, is a **crushing blow** worth +5. A crushing blow can stun a party
member too, so a stunned hero now skips a turn. Players report this:

```
  Ayla HP 40/40 [Normal]
  Bram HP 50/50 [Normal]
  vs
  Troll HP 33/70 [Normal]
Gold 0  Potions 3  > attack
Ayla hits Troll for 13!
Bram hits Troll for 5!
CRUSHING BLOW!
Troll hits Bram for 15!
Bram staggers. Stunned!

  Ayla HP 40/40 [Normal]
  Bram HP 25/50 [Stunned]
```

Bram had 50 HP. The game says he was hit for 15. He has 25 left. Where did
the other 10 go? Do **not** read the code looking for it. Catch it in the
act.

### What changes in your Arena

The planted bug hides somewhere in these changes, so here they are without
the answer: `Fighter` gets `getMaxDamage()`; a new function sits above
`strike`, and `strike` checks for a perfect roll; the monster list starts
with `{"Troll", 70, 8, 10}` and no longer has the Goblin Chief; and the
`attack` loop skips fallen or stunned party members. If you are typing
along, add these to `my-arena/main.cpp`, bug and all. (Or copy the lesson's
`main.cpp`: today is about the debugger, not typing.)

### Warm-up (5 minutes)

Today's `break-it.cpp` doubles as practice. It should print
`Total damage: 30`, but prints `22`. Build it in the lesson folder, with
debug info:

```
cl /nologo /std:c++20 /W4 /EHsc /Zi break-it.cpp                  (Windows)
devenv /DebugExe break-it.exe

g++ -std=c++20 -Wall -Wextra -g break-it.cpp -o break-it          (Linux / macOS)
gdb ./break-it
```

Put a breakpoint on the `total += hits[i];` line (the steps are below),
and watch `i` and `total` each time the program pauses. You will see the
problem on the very first pause. In `gdb`, the breakpoint command is
`break break-it.cpp:13`.

### The hunt, on Windows (Visual Studio)

In Developer PowerShell:

```
cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp
devenv /DebugExe main.exe
```

Visual Studio opens with your program loaded. Then:

1. **File > Open > File...** and open `main.cpp`.
2. Find `int takeDamage(int amount) {` inside the class. Click in the grey
   margin left of it (or put the cursor on it and press **F9**). A red dot
   appears: a breakpoint.
3. Press **F5** to start. A terminal window opens. Type a name, then
   `attack`.
4. The program freezes on your line. Look at the **Locals** window: `amount`
   and `this` (expand it to see `hp`). Open **Debug > Windows > Call Stack**.
   The second line says who called `takeDamage`. Usually it is `strike`.
5. Press **F5** to continue. Keep playing (and continuing) until someone
   lands a crushing blow. The Troll fights first and rolls its maximum (10)
   one time in three, so you will not wait long. Your heroes trigger it
   too, when Ayla rolls 16 or Bram rolls 10. One strike should call
   `takeDamage` once. Watch the call stack: does it?

Tip: pressing F5 at every `takeDamage` gets tiring. Watch the terminal
window instead, and only look closely when it prints `CRUSHING BLOW!`.

The buttons you need:

| Key | Name | What it does |
|---|---|---|
| F5 | Continue | Run until the next breakpoint |
| F10 | Step Over | Run this line, stop on the next one |
| F11 | Step Into | Go inside the function this line calls |
| Shift+F11 | Step Out | Finish this function, stop in the caller |
| Shift+F5 | Stop | End the program |

### The hunt, on Linux or macOS

Use `gdb` (Linux) or `lldb` (macOS) in the terminal, after building with
`-g` as usual. If `gdb` is missing on Linux, install it with your package
manager (on Debian or Ubuntu: `sudo apt install gdb`). On macOS, `lldb`
comes with the Xcode command line tools.

```
gdb ./main                    (or: lldb ./main)
break Fighter::takeDamage     (lldb: b Fighter::takeDamage)
run
bt                            shows the call stack
print amount                  (lldb: p amount)
continue                      (lldb: c)
```

`next` and `step` are Step Over and Step Into. If you prefer clicking, VS
Code with Microsoft's C/C++ extension gives you the same breakpoints and
call stack on top of `gdb` or `lldb`.

## What just happened

You stopped guessing. Instead of adding `std::cout` lines everywhere, you
asked two exact questions: "what is `amount` right now?" and "who called
this?". Once you see `takeDamage` called **twice** for one crushing blow,
the call stack names the culprit, and the fix is one line. (The answer is
folded away at the bottom of this page.)

## Break it

Rebuild `break-it.cpp` **without** debug info:

```
cl /nologo /std:c++20 /W4 /EHsc break-it.cpp                 (Windows: no /Zi)
g++ -std=c++20 -Wall -Wextra break-it.cpp -o break-it        (Linux / macOS: no -g)
```

Open it in the debugger again and set the same breakpoint. Without debug
info, the debugger no longer knows which machine code belongs to which line.
Visual Studio warns that the breakpoint will not be hit because no symbols
were loaded. `gdb` answers `break break-it.cpp:13` with:

```
No symbol table is loaded.  Use the "file" command.
```

It can still stop on a function by name (`break main`), but then
`info locals` says `No symbol table info available.` Your lines and
variable names are gone.

Lesson: **debug info is part of the build.** If the debugger acts strange,
first check that you built with `/Zi` or `-g`, and that you are running the
`.exe` you just built.

## Try it yourself

1. In the warm-up, use **Step Over** (F10) to walk through the whole loop and
   count how many times it runs.
2. Fix the Arena bug, rebuild, and confirm the numbers now add up.

## Going further

Right-click a breakpoint and choose **Conditions** to stop only when, say,
`amount > 12` (`gdb`: `condition 1 amount > 12`). That would have made
today's hunt even faster. In Part 2 you will use the same debugger on your
Unreal game.

<details>
<summary>Spoiler: the bug and the fix</summary>

`addCrushingBonus` is supposed to *return* the bigger number. It also calls
`defender.takeDamage(roll)` itself, and then `strike` calls `takeDamage`
again with the bonus. One crushing blow lands twice: 10, then 15. Delete the
`defender.takeDamage(roll);` line from `addCrushingBonus`. Lesson 13's
`main.cpp` has the fixed version.

</details>

**Checkpoint: you can now** pause a program at a breakpoint, read its
variables, step line by line, and use the call stack to see who called what.

Next: [13-pointers](../13-pointers/) - the hard one, taken slowly.
