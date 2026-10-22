# 34 - Capstone: Orb Arena 1.0

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Turn Orb Arena into a real, finished game: enemies with states, a round
timer, winning and losing, a restart key, a packaged build that runs on
another PC, and one feature that is entirely yours.

This lesson is **not** bite-size. Plan **several sittings of 30-60 minutes**,
about 3-5 hours in total. There is no big new topic: it is putting together
what you already know, the way real game development works. Most of the
time goes into small problems, and solving them is the skill.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
>
> **Save before you press Play** (Ctrl+Shift+S).

## How this lesson works

- It is a **checklist of milestones**. Do them in order. Each one says what
  to build, which earlier lessons it uses, and how to check it works.
- **Save a checkpoint after each milestone.** If you use git, commit (only
  `Source/`, `Config/`, `Content/` and the `.uproject`, never the generated
  folders from PREP.md). If not, copy your `Source` folder and the level to a
  backup folder named after the milestone. When something goes badly wrong
  later, you go back one milestone, not to the start.
- This folder's `Source/` holds a **finished version** of every file that
  changes, with **all** milestones in it. Try each milestone yourself first,
  from the description. Look at the solution files when you are stuck, or to
  compare afterwards. Copying without trying first teaches very little.
- **Careful when you peek early.** A finished file can use functions that a
  later milestone adds, and copying it too early breaks your build (for
  example `'IsRoundOver': is not a member of 'AOrbArenaGameMode'` at
  milestone 1). So every milestone below names the exact files to compare
  with. Where a file changes in more than one milestone, the `milestones/`
  folder holds a snapshot of it as it should look at the end of that
  milestone. Every addition is tagged with its milestone, such as
  `NEW (milestone 3)`. In `OrbArenaCharacter.additions.txt`, add only the
  sections for your milestone and the ones before it.
- Stop at the end of any milestone. They are good places to rest.

| Milestone | Compare with these files |
|---|---|
| 1 | `Source/OrbArena/OrbTypes.h`, `milestones/after-milestone-1/Enemy.h` and `Enemy.cpp`, character sections 1A-1C |
| 2 | none: it is yours (but see the outline) |
| 3 | `milestones/after-milestone-3/OrbArenaGameMode.h` and `.cpp`, `Source/OrbArena/OrbSpawner.h` and `.cpp`, `Source/OrbArena/Enemy.h` and `Enemy.cpp`, character section 3A |
| 4 | `Source/OrbArena/OrbHUDWidget.h` and `.cpp`, `milestones/after-milestone-4/OrbArenaGameMode.cpp` |
| 5 | `Source/OrbArena/OrbArenaGameMode.h` and `.cpp` (finished), character sections 5A-5D |
| 6, 7 | no code files |

> **What you can ignore for now**
>
> - Polish beyond the milestones. A finished small game beats an unfinished
>   big one. Ideas go on a list for "version 1.1".
> - Anything about multiplayer, animation, or AI path finding.

## Milestone 1: enemy states

**Build:** the enemy gets three states, **Idle** (waits until the player is
within sight range), **Chase** (lesson 30's chase), and **Stunned** (frozen
for 2 seconds). The player's dash stuns any enemy close by.

**How:**

- Add `EEnemyState` (`Idle`, `Chase`, `Stunned`) to `OrbTypes.h`, as a
  `UENUM(BlueprintType)` (lesson 27).
- In `AEnemy`, a `State` member and a `switch (State)` in `Tick` with one
  `case` per state (lesson 09). Each case does its job and decides whether
  to change state. Put every change through one `SetState` function, so
  there is one place to log and debug.
- `AEnemy::Stun(float Seconds)` sets the state and a countdown.
- In the character's `Dash()` (lesson 23), find enemies with
  `GetAllActorsOfClass` (lesson 26) and stun the ones within `StunRadius`.

**Files:** `Source/OrbArena/OrbTypes.h`; `Enemy.h` and `Enemy.cpp` from
`milestones/after-milestone-1/` (**not** the finished ones in `Source/`, which
need milestone 3); and the character (`OrbArenaCharacter.additions.txt`,
sections 1A, 1B, 1C). At this point the enemy still only logs "Caught!":
catching ends the round from milestone 3 on.

**Check:** select an enemy in the Outliner during Play and watch **State**
in the Details panel. Walk close: Idle becomes Chase. Dash next to it: it
squashes into a low, flat box (Stunned), then chases again.

This is the Text Arena's monster stance from lesson 09, now in 3D.

## Milestone 2: enemy waves

**Build:** enemies appear over time, more of them as the round goes on.

**How:** this one is yours to write, using lesson 29 as the model. Make an
`AEnemySpawner` with a `TSubclassOf<AEnemy> EnemyClass`, a timer, and
`SpawnActor`. To ramp up, spawn one more enemy each wave, or make the
interval shorter each time.

If you do not know where to start, follow this outline:

1. Close the editor and Visual Studio. In File Explorer (with file name
   extensions shown, lesson 27), copy your `OrbSpawner.h` and
   `OrbSpawner.cpp` and rename the copies `EnemySpawner.h` and
   `EnemySpawner.cpp`.
2. Open the copies. With **Match case** on, replace every `OrbSpawner` with
   `EnemySpawner` (this also fixes the `#include "EnemySpawner.h"` and
   `"EnemySpawner.generated.h"` lines and the class name `AEnemySpawner`).
   Then replace `#include "Orb.h"` with `#include "Enemy.h"`, `AOrb` with
   `AEnemy` (tick **Match whole word** too), and `OrbClass` with
   `EnemyClass`. Delete the `SetKind` line: enemies have no kind.
3. Ramp it up: rename `OrbsPerWave` to `EnemiesThisWave`, start it at `1`,
   loop `EnemiesThisWave` times in `SpawnWave`, and add
   `EnemiesThisWave++;` at the end of each wave.
4. Right-click `OrbArena.uproject` and choose **Generate Visual Studio
   project files**. Open the `.sln` and build. Fix the first error, then
   build again.
5. In the editor, drag an **EnemySpawner** into the level and set **Enemy
   Class** to **Enemy**.

Two rules for later: **do not** call `RegisterSpawner()` from the enemy
spawner (in milestone 3 that would make the round unwinnable, because the
referee would wait for it forever), and spawn enemies away from the player
start, or the first one catches you at once.

**Files:** your own `EnemySpawner.h` and `EnemySpawner.cpp`. (Not in this
folder on purpose.)

**Check:** a round starts calm and gets busy. Spawned enemies chase (that is
why lesson 30 used `PlacedInWorldOrSpawned`).

## Milestone 3: round timer, winning and losing

**Build:** the GameMode runs a countdown. You **win** when every spawner has
finished and every orb is collected. You **lose** if an enemy catches you or
the time runs out. When the round ends, the player stops moving, enemies
freeze, and nothing more counts.

**How:**

- A timer in the GameMode that calls `TickRound` once per second (lesson
  29), counting `TimeLeft` down.
- Two new delegates (lesson 31): `FOnTimeChanged` (an `int32`) and
  `FOnGameOver` (a `bool bWon`).
- **One** function ends the round, `EndRound(bool bWon)`, and it ignores
  every call after the first. (The clock and an enemy might both try in the
  same second.)
- **Orb** spawners **register** with the GameMode and report when their last
  wave is out, the same self-registration pattern as the orbs in lesson 29.
  That is how the referee knows "no more orbs are coming". Your enemy
  spawner from milestone 2 does **not** register; it should only stop
  spawning once `IsRoundOver()` is true, the same way the finished
  `OrbSpawner::SpawnWave` checks it.
- Enemies call `GameMode->PlayerCaught()` instead of just logging (and
  `bHasCaughtPlayer` can go).
- In the character's `Dash()`, return early when the round is over.
- `PlayerController->SetIgnoreMoveInput(true)` stops the player walking.

**Files:** `OrbArenaGameMode.h` and `.cpp` from
`milestones/after-milestone-3/`; `OrbSpawner.h`, `OrbSpawner.cpp`, `Enemy.h`
and `Enemy.cpp` from `Source/OrbArena/`; character section 3A.

**Check:** play three rounds: let the time run out, get caught, and collect
everything. Each one logs `Round over: ...` exactly once.

## Milestone 4: HUD polish

**Build:** the HUD shows the time left, and a game-over message that says
whether you won.

**How:** two more `BindWidget` text blocks in `UOrbHUDWidget`, `TimeText`
and `GameOverText` (lesson 33), with `SetTime` and `ShowGameOver` subscribed
to the new delegates. `NativeConstruct` hides the game-over text at the
start. In `WBP_HUD`, add two Text widgets named exactly `TimeText` and
`GameOverText`, and put the game-over text in the middle of the screen with
a big font.

**Files:** `OrbHUDWidget.h` and `.cpp` from `Source/OrbArena/`;
`OrbArenaGameMode.cpp` from `milestones/after-milestone-4/`; and `WBP_HUD`.
(Build the C++ first, then add the two Text widgets: a `BindWidget` without
its widget makes `WBP_HUD` refuse to compile, as in lesson 33's Break it.)

**Check:** the clock counts down (`Time: 1:29`, `1:28`...). The message
appears at the end, and only then.

## Milestone 5: restart

**Build:** after a round ends, pressing **R** starts a fresh round.

**How:**

- A new Input Action `IA_Restart`, mapped to **R** in the mapping context
  (lesson 23), bound to a `RestartGame()` function on the character.
- `RestartGame()` asks the GameMode, and the GameMode's `RestartRound()`
  does nothing unless the round is over. Otherwise it reloads the current
  level with `UGameplayStatics::OpenLevel` and
  `UGameplayStatics::GetCurrentLevelName`. Reloading throws away every Actor
  and creates them fresh: the simplest possible reset.
- In the character Blueprint's Class Defaults, set **Restart Action** to
  `IA_Restart`, then compile and save.

A naming lesson hides here: the obvious name `Restart()` is already a
`virtual` function in `APawn`, a parent of every character. Writing your own
`void Restart()` would silently **override** the engine's (lesson 15) and
break it. The comment in `OrbArenaCharacter.additions.txt` explains.

**Files:** the finished `OrbArenaGameMode.h` and `.cpp` from
`Source/OrbArena/`; character sections 5A, 5B, 5C, 5D.

**Check:** win, lose, restart, again. Score and time are back to the start
every time. Pressing **R** in the middle of a round does nothing.

## Milestone 6: package it

**Build:** a real Windows build of your game, a folder with an `.exe`, that
runs without the editor, on someone else's PC.

**How:**

1. **Edit > Project Settings > Maps & Modes:** set **Game Default Map** to
   your arena level. (Keep this in mind; it is the bug hunt below.)
2. **Edit > Project Settings > Packaging:** for a first build, keep the
   **Build Configuration** on **Development**. It still writes a log file,
   which you will need. (A **Shipping** build is smaller and faster but
   writes almost nothing to help you debug. It also leaves out all debug
   drawing and debug text: the tracker's green line, the scanner's line,
   and every `AddOnScreenDebugMessage`. If those vanish in a Shipping
   build, your game is not broken. Your UMG HUD still shows.)
3. In the editor toolbar, open the **Platforms** menu, choose **Windows**,
   then **Package Project**. Pick an empty folder outside your project, such as
   `C:\UnrealBuilds\OrbArena`.
4. Wait. The first package takes a long time: Unreal compiles your code
   again for a game without the editor, and "cooks" every asset into a
   game-ready format. Watch the **Output Log** for progress.
5. Open the output folder (it contains a `Windows` folder) and run
   `OrbArena.exe`.

**The packaged-build bug hunt.** "It works in the editor but not when
packaged" is one of the most common problems in real game development. Meet
it on purpose:

1. Choose **File > Save Current Level As** and save your arena as a new
   level named `OrbArenaLevel`. Keep working in it.
2. Do **not** change **Game Default Map**. Package again and run the `.exe`.
3. You land in the wrong level: the old one, without your latest changes.
   Nothing crashed, nothing complained.
4. Find the evidence. A Development build writes a log file inside the
   packaged folder, under `Windows\OrbArena\Saved\Logs\OrbArena.log`. Open
   it in a text editor and search for `map` or `Load`. You will find a line
   that names the map the game loaded at startup, and it is not
   `OrbArenaLevel`.
5. Fix it: set **Game Default Map** to `OrbArenaLevel`, package again, and
   run.

Lesson: the editor plays **the level you have open**; a packaged game plays
**the level in its settings**. When a packaged build misbehaves, its log
file is your Output Log.

**Check:** zip the output folder, copy it to another Windows PC, unzip and
play. (If it complains about a missing runtime, the other PC may need the
Microsoft Visual C++ Redistributable or DirectX runtime installed. Read the
exact message.)

## Milestone 7: performance check

**Build:** know your frame rate, and stop doing work you do not need.

**How:**

- During Play, open the console (the **`` ` ``** key) and type `stat fps`, then
  `stat unit`. `stat unit` splits each frame's time into **Game** (your C++
  and Blueprints), **Draw** and **GPU**. If Game is the biggest, your code is
  the bottleneck.
- Spawn 200 orbs (change the spawner settings) and watch the numbers.
- Turn off `Tick` where it is not needed: `PrimaryActorTick.bCanEverTick =
  false;` in the constructor. The spawner already does this. Does the
  tracker still need to search every frame?
- For spinning, Unreal has a ready-made `URotatingMovementComponent` that
  spins an Actor without your own `Tick`. Look it up and try it on the orb.

**Check:** with 200 orbs, write down your `stat fps` before and after one
improvement.

## Your own feature (pick at least one)

This part has no finished files. It is yours.

- **A power-up orb:** a new `EOrbKind` that makes the player faster for 5
  seconds (`GetCharacterMovement()->MaxWalkSpeed`, a timer to put it back).
- **A high score that survives a restart:** see
  [bonus-save-game](../bonus-save-game/).
- **A scanner** that highlights the orb you look at: see
  [bonus-line-trace](../bonus-line-trace/).
- **Splitting enemies:** when stunned twice, an enemy splits into two smaller
  ones, like the Text Arena's Slime.
- **A second arena layout,** chosen at random on restart.
- **Music** that changes when an enemy is chasing.
- **Something you thought of.** Write down in two sentences what it is
  before you start, so you know when it is done.

## Break it

The packaged-build bug hunt in milestone 6 is this lesson's "Break it":
a game that is wrong without any error, diagnosed from a log file. One more
for the road, if you want it: in `EndRound`, comment out the
`if (bRoundOver) return;` guard, and in `AEnemy::Tick`, comment out the
`IsRoundOver()` check. Only function bodies changed, so use Live Coding.
Then Play and let an enemy catch you. The Output
Log fills with `Round over: you lose` once **per frame**, because the enemy
keeps catching you and nothing stops the round from ending again and again.
Put both checks back. Guards like these are how "only once" really means
once.

## Stuck?

| Symptom | Where to look |
|---|---|
| The game never says "you win" | Does your enemy spawner call `RegisterSpawner()`? It must not (milestone 2). Is an orb out of reach (inside a block, or under the floor), still counted in `OrbsLeft`? Watch `OrbsLeft` in the GameMode's Details panel during Play. |
| `'IsRoundOver': is not a member of 'AOrbArenaGameMode'` (or `SetTime`, `RestartRound`) | You copied a file from a later milestone. Use the files the milestone table names. |
| **R** does nothing | Is `IA_Restart` mapped in the mapping context (lesson 23's Break it)? Is **Restart Action** set in the character Blueprint? Is the round actually over? |
| Enemies never move | Check their **State** in the Details panel. Is the player within **Sight Range**? Is the round already over? |
| HUD shows "Score" but no time | Is `TimeText` named exactly that in `WBP_HUD`? Did the Widget Blueprint compile? |
| Editor will not open after a change | The clean rebuild ritual in [PREP.md](../19-unreal-hello/PREP.md). |
| Packaging fails | Read the **first** error in the Output Log, the same skill as lesson 01. Search it online with the word "Unreal". |
| Something strange you cannot explain | A breakpoint (lesson 12) and the call stack. Then the stuck ladder from lesson 00. |

## Done means

- The **packaged** game runs on another PC.
- A round can be **won**, **lost** (both ways), and **restarted**.
- You wrote **one paragraph**: one thing you built that you are proud of,
  and one bug you fixed (what you saw, what caused it, how you found it).

That paragraph matters. Being able to explain a bug you fixed is what
programmers do in every job interview, and it is how you notice how much you
have learned since lesson 01's missing semicolon.

## Going further

You finished. Some directions from here:

- The optional lessons [bonus-line-trace](../bonus-line-trace/) and
  [bonus-save-game](../bonus-save-game/).
- Real path finding with a Nav Mesh and an AI controller's `MoveToActor`.
- Animated characters instead of boxes (the template's mannequin is a good
  start).
- Read the template's character code again, top to bottom. Notice how much
  of it you can read now.

**Part 1 link:** all of it. Lesson 09's stance enum became enemy states,
lesson 18's spawner became two spawners, lesson 16's lambdas became
delegates, and the Text Arena's game loop became `Tick`.
