# 19 - Unreal says hello

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

**Before this lesson:** do [PREP.md](PREP.md), ideally the day before. It
installs the engine and creates the `OrbArena` project. This lesson assumes
you pressed Play once and walked around.

## Goal

See one line of your own C++ appear inside Unreal's Output Log.

## Welcome to Part 2

Open the `Source` folder of your project and you will see files full of
words you have never met: `UCLASS`, `GENERATED_BODY`, `TObjectPtr`,
`UInputMappingContext`. The Content Browser in the editor is full of assets.
The menus have menus.

**That is normal, and you do not need to understand it.** Professional
Unreal developers do not know the whole engine either. In every Part 2
lesson, you only need to understand **the lines the lesson points at**.
Everything else is someone else's code that works. You will learn to read
more of it, one lesson at a time.

> **What you can ignore for now**
>
> - Every file in `Source/OrbArena/` except `OrbArenaGameMode.h` and
>   `OrbArenaGameMode.cpp`.
> - The `OrbArena.Build.cs` and `*.Target.cs` files. (They are settings
>   files for Unreal's build tool. Lesson 33 touches one line.)
> - The line `#include "OrbArenaGameMode.generated.h"`. Lesson 20 explains it.
> - `CoreMinimal.h`. It is "the basics of Unreal", like `<iostream>` was the
>   basics of printing.
> - Everything in the Content Browser.
> - If your template made extra folders in `Source/OrbArena/` with names
>   starting with `Variant_` (for example `Variant_Combat`,
>   `Variant_Platforming`, `Variant_SideScrolling`), they are example game
>   styles from Epic. They are not part of this course. Leave them alone.
>
> If something on your screen does not match a lesson exactly, read
> "If something differs from these lessons" at the end of [PREP.md](PREP.md).

## The new idea

- **Unreal owns `main`.** In Part 1 your program started at `main()`. Here,
  Unreal is a huge C++ program that has its own `main` somewhere deep inside.
  It loads *your* code and **calls your functions** at the right moments.
  JavaScript frameworks work the same way: you do not call React's render
  loop, React calls your component.
- **You join in by overriding.** The engine calls `BeginPlay()` on every
  Actor when the game starts. You override it with your own version, exactly
  like lesson 15's `virtual` and `override`.
- **`Super::BeginPlay()` calls the parent's version first.** In lesson 15 you
  wrote `Monster::takeTurn(party)` to run the base class's code. Unreal gives
  every class a nickname for its parent, `Super`, so you write
  `Super::BeginPlay()`. Forgetting it is a classic bug: the engine's own
  setup never runs.
- **`UE_LOG` is `console.log`.** It writes a line to the Output Log.

```cpp
UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));
//     ^        ^        ^
//     |        |        TEXT() turns "..." into Unreal's kind of text
//     |        how loud: Log, Warning (yellow), or Error (red)
//     a category name, so you can filter the log
```

## Two rules for all of Part 2

**1. The rebuild rule.**

- Changed **only the inside of a function** in a `.cpp` file? With the
  editor open, use **Live Coding**: press **Ctrl+Alt+F11**, or click the
  **Live Coding** (compile) button at the bottom right of the editor.
- Changed a **`.h` file**, added a `UPROPERTY` or `UFUNCTION`, or added a
  **new class**? **Close the editor**, build in Visual Studio, then reopen
  the editor. Later lessons call this **close and rebuild**.

If you build in Visual Studio while the editor is open, you may get
"Unable to build while Live Coding is active". That is the rule reminding
you: close the editor first.

A Live Coding change lives only in the editor that is open right now. The
next time you open the project, Unreal may notice your code is newer than
its last real build and ask whether to rebuild modules. Click **Yes**, or,
better, build once in Visual Studio before you open the editor. Either way,
nothing is broken.

**This lesson changes a header**, so you will close the editor and build in
Visual Studio.

**2. Save before you press Play.** Press **Ctrl+Shift+S** (**File > Save
All**) before every Play. When your C++ has a bug, the whole editor can crash,
and unsaved work in the level is gone.

## Try it

1. **Close the Unreal editor.**
2. Open `OrbArena.sln` in Visual Studio 2022 (see [PREP.md](PREP.md) step 5).
3. At the top, check the configuration is **Development Editor** and the
   platform is **Win64**.
4. In **Solution Explorer**, open `Games > OrbArena > Source > OrbArena >
   OrbArenaGameMode.h`.

This file came with the template, so **do not replace it**. Your copy may
have a few more lines than ours, and those lines matter. **Add** only the
lines marked `NEW` in this folder's `Source/OrbArena/OrbArenaGameMode.h`.
Inside the class, after the `public:` section, add:

```cpp
protected:
	// NEW (lesson 19): the engine calls this once when the game starts.
	virtual void BeginPlay() override;
```

5. Open `OrbArenaGameMode.cpp`. At the bottom of the file, add:

```cpp
void AOrbArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));
}
```

6. In Solution Explorer, right-click the **OrbArena** project and choose
   **Build**. The first build after opening can take a minute. At the end
   the Output window says something like:

```
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

7. Open the editor: double-click `OrbArena.uproject` (or press **F5** in
   Visual Studio, which starts the editor with the debugger attached).
8. Open **Window > Output Log**. Save (**Ctrl+Shift+S**), then click **Play**.
9. In the Output Log, type `Orb` in the search box. You should see:

```
LogTemp: Warning: Orb Arena is running!
```

That yellow line is yours. Your C++ is running inside a 3D game engine.

**Nothing in the log?** The level may be using a different GameMode. Open
**Window > World Settings** and look at **GameMode Override** in the Game
Mode section. If it names a Blueprint such as `BP_ThirdPersonGameMode`, that
is fine: it is a Blueprint *child* of your C++ class, so it runs your
`BeginPlay` too. If it names something unrelated, clear it, and check
**Edit > Project Settings > Maps & Modes > Default GameMode**.

## What just happened

1. You added an **override** of `BeginPlay` to the GameMode class. The
   GameMode is a "referee" object Unreal creates for every level (lesson 25
   gives it real work).
2. Visual Studio ran Unreal's build tool, which compiled your `.cpp` into a
   `.dll` file in `Binaries/`.
3. The editor loaded that `.dll`. When you pressed Play, the engine created
   the GameMode and called its `BeginPlay`. Because it is `virtual`, your
   version ran. Your version first called `Super::BeginPlay()` (the engine's
   setup), then logged your line.

## Break it

Errors in Unreal builds look scarier, but they are the same compiler errors
as in Part 1. Make one on purpose.

1. Close the editor.
2. In `OrbArenaGameMode.cpp`, delete the `;` after `Super::BeginPlay()`.
3. Build the **OrbArena** project in Visual Studio.

The **Error List** window may show a confusing mix, including red squiggle
errors from IntelliSense (Visual Studio's guesser) that are not real. Ignore
it for now. Open **View > Output** instead and set **Show output from** to
**Build**. This is the **full build log**, and it is the truth. Scroll to the
first line that says `error`. It looks something like this:

```
...\Source\OrbArena\OrbArenaGameMode.cpp(14): error C2143: syntax error: missing ';' before ...
```

Same three parts as lesson 01: **where** (file and line), **what** (error
code C2143, missing `;`), and a hint. The word after "before" may look odd,
because `UE_LOG` is a macro: the compiler sees the code it expands into, not
the word `UE_LOG`. As in lesson 01, the problem is on the line it names or
the line just above.

Put the `;` back and build again.

**Tip:** if you prefer the Error List, change its filter dropdown from
"Build + IntelliSense" to **"Build Only"**. Then it shows only real errors.

## Try it yourself

1. With the editor **open**, change the text of your message. You changed
   only a function body, so use **Live Coding** (**Ctrl+Alt+F11**). Wait for
   the "Live coding succeeded" notice, press **Play**, and find your new
   message.
2. Log a number, the way `printf` does it:
   `UE_LOG(LogTemp, Warning, TEXT("Arena size: %d"), 42);`
3. Change `Warning` to `Error` and see the line turn red.

## Going further

- Print to the game screen instead of the log. Add `#include
  "Engine/Engine.h"` at the top of the `.cpp`, then:
  `if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
  TEXT("Hello from C++")); }`
- Use lesson 12's debugger. Click in the margin next to the `UE_LOG` line to
  set a breakpoint, press **F5** in Visual Studio to start the editor under
  the debugger, then press Play. The game freezes on your line, and you can
  look at the call stack: that is the engine calling you. Your own functions
  have names; the engine's show as bare addresses in `.dll` files, because
  you skipped the engine's debugging symbols in PREP. That is fine.

**Part 1 link:** 01 (reading the first error), 15 (`virtual`, `override`,
calling the base version).

Next: [20-your-first-actor](../20-your-first-actor/) - write your own Actor
class and watch it spin.
