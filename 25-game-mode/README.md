# 25 - The referee

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Keep the score in one place, the GameMode, and have each orb report to it.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson changes the GameMode's header:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S). This lesson's "Break it"
> crashes the editor on purpose, so it really matters today.

## The new idea

- **The GameMode is the referee.** Unreal creates exactly one GameMode for
  each level you play. It is the natural home for the rules: the score, when
  you win, when you lose. Orbs do not keep score themselves; they **report**
  to the referee.
- **One place for the rules.** In JavaScript you might keep `let score = 0`
  as a global that any file changes. It works until three files change it
  differently. A single `game` object with an `addScore()` method is easier
  to trust and to debug. Same here: `AddScore` is the only function that
  changes `Score`.
- **Finding the referee:** `GetWorld()->GetAuthGameMode<AOrbArenaGameMode>()`
  asks the world for its GameMode, as your class. It returns `nullptr` if
  the level uses some other GameMode. So: check it (lesson 13).

> **What you can ignore for now**
>
> - The word "Auth" (authority). It matters only for multiplayer.
> - `GameStateBase`, `PlayerState`, `PlayerController`: other referee-like
>   classes for bigger games.
> - The other things the template's GameMode may do, such as picking which
>   character the player controls.

## Which GameMode does your level use?

You checked this briefly in lesson 19. Look again: **Window > World
Settings**, section **Game Mode**, field **GameMode Override**. (If it is
empty, the level uses **Edit > Project Settings > Maps & Modes > Default
GameMode**.)

- If it is a **Blueprint** such as `BP_ThirdPersonGameMode`, open it and
  check that its parent class (top right of the Blueprint editor) is
  `OrbArenaGameMode`. That is the best case: from lesson 26 on, you will
  tune the referee's settings in that Blueprint's Class Defaults.
- If it is the **C++ class** `OrbArenaGameMode` directly, make a Blueprint
  child now: in the Content Browser, open **C++ Classes > OrbArena**,
  right-click **OrbArenaGameMode**, choose **Create Blueprint class based on
  OrbArenaGameMode**, and name it `BP_OrbGameMode`. Open it and set **Default
  Pawn Class** to the template's character Blueprint
  (`BP_ThirdPersonCharacter`). Also set **Player Controller Class** to the
  template's player controller Blueprint, if your template has one (search
  the Content Browser for `PlayerController`; it may be called
  `BP_ThirdPersonPlayerController`). Compile, save, and choose it as the
  **GameMode Override** in World Settings.

> **Why the player controller matters.** In recent templates, the player
> controller is the object that switches on the keyboard and mouse controls
> (it adds the input mapping context from lesson 23). A GameMode that uses
> the plain default controller can give you a character that **cannot
> move**. If you ever press Play and W A S D does nothing, check the
> GameMode's **Player Controller Class** first. (In older templates the
> character does this job itself, and the controller does not matter.)

## Try it

1. Close the editor.
2. In `OrbArenaGameMode.h`, add `AddScore`, `GetScore` and the `Score`
   property (lines marked NEW in this folder's file). Keep your template's
   other lines.
3. In `OrbArenaGameMode.cpp`, add the `AddScore` function.
4. In `Orb.cpp`, add the two includes and replace the inside of
   `HandleOverlap` with this folder's version. The key part:

```cpp
AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();
if (GameMode != nullptr)
{
	GameMode->AddScore(1);
}
```

5. Build, then open the editor, save, Play. Collect three orbs:

```
LogTemp: Warning: Score: 1
LogTemp: Warning: Score: 2
LogTemp: Warning: Score: 3
```

## What just happened

1. When the level started, Unreal created one GameMode object, from your
   class or its Blueprint child.
2. Each orb, when touched, asked the world for that GameMode with
   `GetAuthGameMode<AOrbArenaGameMode>()`. Because the real object is your
   class (or a child of it), the cast worked and returned a pointer.
3. The orb called `AddScore(1)`. Only the GameMode touched `Score`.

## Break it

This one **crashes the editor** on purpose. Save everything first
(**Ctrl+Shift+S**), and know that it is safe: nothing is damaged, and you will
reopen the project in a minute.

1. Close the editor. In `Orb.cpp`, remove the safety check so the code
   trusts the pointer blindly:

```cpp
AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();
GameMode->AddScore(1);   // no check!
```

2. Build, then open the editor.
3. Make a GameMode that is **not** yours, but that still lets you walk.
   The easy way is to copy the GameMode Blueprint you already use and cut
   it loose from your C++ class:
   - In the Content Browser, right-click your GameMode Blueprint (for
     example `BP_ThirdPersonGameMode` or `BP_OrbGameMode`) > **Duplicate**.
     Name the copy `BP_OtherGameMode`.
   - Open the copy. Click **Class Settings** in the toolbar. (If it opens
     as a simple settings list with no toolbar buttons, it is a data-only
     Blueprint: click **Open Full Blueprint Editor** first, see lesson 23,
     step 7.) In the Details panel, change **Parent Class** to **Game Mode
     Base**. Confirm the warning. Compile and save.
   - Still in the copy, check that **Default Pawn Class** and **Player
     Controller Class** still name the template's character and player
     controller. If one went back to a default, set it again (see the box
     "Why the player controller matters" above).
4. In **World Settings**, set **GameMode Override** to `BP_OtherGameMode`.
5. Save (**Ctrl+Shift+S**), Play, and walk into an orb. (If you cannot walk at
   all, the player controller is wrong: fix step 3 before going on.)

The editor freezes and the **Crash Reporter** window opens. (If you
started the editor from Visual Studio with **F5**, Visual Studio catches the
crash instead and shows you the exact line. That is an even better view:
look at its **Call Stack** window, then press **Shift+F5** to stop.) Near
the top the Crash Reporter says something like:

```
Unhandled Exception: EXCEPTION_ACCESS_VIOLATION writing address 0x00000000000002c8
```

(your exact number will differ), followed by a list of function names: the
**call stack**. Read it from the top. You should find `AOrb::HandleOverlap`
and `AOrbArenaGameMode::AddScore` near the top. That is where it happened.

Decode it: "access violation" means the program touched memory it does not
own. An address **very close to zero** almost always means **a `nullptr`
was used**: `AddScore` tried to change `Score`, which lives a few hundred
bytes past where the GameMode would start, and the GameMode was at address
0. This is lesson 13's crash, in an engine.

Close the Crash Reporter with **Close Without Sending**. When you open the
project again, Unreal may offer to restore unsaved packages: click **Skip**
(you saved before Play, so there is nothing to restore).

Fix it: put the `if (GameMode != nullptr)` check back, build, reopen the
editor, and set the **GameMode Override** back to your own GameMode. Play once
more with `BP_OtherGameMode` to see your red error message instead of a crash.
Then switch back.

Lesson: **things set in the editor can change under your code**. Every
pointer you get from a cast or a lookup gets checked.

## Try it yourself

1. Give orbs a `Points` property (`UPROPERTY(EditAnywhere, Category =
   "Orb") int32 Points = 1;` in `Orb.h`) and call `AddScore(Points)`. Make
   one orb worth 10.
2. Watch the score live: during Play, find the GameMode in the **Outliner**
   (the list of Actors in the level), click it, and watch **Score** change in
   the Details panel.
3. Add a `bool bGameOver` to the GameMode, and stop counting once the score
   reaches 5.
4. See the score in the game, not just in the log: in `AddScore`, print it
   on screen with `GEngine->AddOnScreenDebugMessage` (lesson 19's "Going
   further" shows how, including the `#include`). This is only for you: the
   lesson files keep using the log, and lesson 33 builds a real HUD.

## Going further

Bigger games split the referee into two: the **GameMode** (the rules, only
on the server) and the **GameState** (the scoreboard everyone can see). For
a single-player game, the GameMode alone is fine.

**Part 1 link:** 11 (a class protects its data), 13 (null pointer checks).

Next: [26-tarray-and-tmap](../26-tarray-and-tmap/) - give orbs different
values, and declare a win.
