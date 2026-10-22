# 29 - Things appear over time

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Make orbs appear in waves, every few seconds, instead of placing them all by
hand.

**Time:** about 40 minutes (a new class plus several rebuilds). There is a
marked good place to stop before "Break it".

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a new class and changes two headers:** close and
> rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **`SpawnActor` creates an Actor while the game runs.** Lesson 18's
  `Spawner` created monsters with `std::make_unique` and the arena owned
  them. `GetWorld()->SpawnActor<AOrb>(OrbClass, Location, Rotation)` also
  creates an object at runtime, but **the engine owns it**. You get back a
  plain pointer to look at it. You never delete it; you `Destroy()` it when
  it should leave the game (lesson 28).
- **`TSubclassOf<AOrb>` holds a class, not an object.** "Which kind of orb
  should I make?" You pick it in the editor. It can be `AOrb` itself or any
  child of it, such as a Blueprint you will make in lesson 32.
- **Timers call a function later, or again and again.**
  `GetWorldTimerManager().SetTimer(WaveTimer, this, &AOrbSpawner::SpawnWave,
  10.f, true);` is JavaScript's `setInterval(spawnWave, 10000)`. The
  `FTimerHandle` is your ticket: keep it to stop the timer, like
  `clearInterval`.
- **The counting moves into the orbs.** Each orb now reports itself to the
  GameMode in its own `BeginPlay` (`RegisterOrb`). Unreal does not promise
  which Actor's `BeginPlay` runs first, so lesson 26's "count everything at
  the start" could miss or double-count spawned orbs. Self-registration
  works in any order.

> **What you can ignore for now**
>
> - `meta = (ClampMin = "1")`: stops the editor from accepting silly values.
> - `FRotator::ZeroRotator`: "no rotation".
> - `FMath::FRandRange` and `FMath::RandRange`: random numbers, like lesson
>   04's dice machine, for decimals and whole numbers.

**One new piece of syntax.** In `Orb.cpp` you will see
`if (AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>())`.
That declares a variable *inside* the `if` and checks it in one go: the body
runs only if the pointer is not `nullptr`. It is a short way to write the two
lines you know from lesson 25 (get the pointer, then
`if (GameMode != nullptr)`), and `GameMode` exists only inside that `if`.

## Try it

1. In the editor, choose **Tools > New C++ Class**, pick **Actor**, and name
   it **`OrbSpawner`**. Then close the editor.
2. Replace `OrbSpawner.h` and `OrbSpawner.cpp` with this folder's versions.
   The two key moments:

```cpp
// BeginPlay: start a repeating timer
GetWorldTimerManager().SetTimer(WaveTimer, this, &AOrbSpawner::SpawnWave,
	SecondsBetweenWaves, true);

// SpawnWave: create one orb
AOrb* Orb = GetWorld()->SpawnActor<AOrb>(OrbClass, Location, FRotator::ZeroRotator);
```

3. **Orb:** add the public `SetKind` function and the `RegisterOrb` call in
   `BeginPlay` (lines marked NEW in this folder's `Orb.h` and `Orb.cpp`).
4. **GameMode:** add `RegisterOrb`, and remove lesson 26's
   `GetAllActorsOfClass` count from `BeginPlay`.
5. Build, then open the editor.
6. Drag an **OrbSpawner** into an open, flat part of the level, about 1
   meter above the floor. In its **Details** panel, set **Orb Class** to
   **Orb**. Like the tracker, it is invisible: select it again from the
   **Outliner**. Orbs appear anywhere in a square up to **Radius** (8 m)
   from it, so keep it away from the template's blocks and ramps, or lower
   **Radius**. An orb that spawns inside a block can never be collected.
7. Save and Play. Orbs appear around the spawner, five at a time, every 10
   seconds, three waves:

```
LogTemp: Warning: Wave 1 of 3: 5 orbs!
LogTemp: Warning: Score: 1
...
LogTemp: Warning: Wave 2 of 3: 5 orbs!
```

You might also see "You win!" between waves, if you collect a wave before
the next one arrives. The referee does not yet know more waves are coming.
The capstone (lesson 34) turns this into real rules.

## What just happened

1. At `BeginPlay`, the spawner checked that `OrbClass` was set, spawned the
   first wave, and started a repeating timer.
2. Every 10 seconds, the engine's timer manager called `SpawnWave` on the
   spawner. You never wrote a loop that waits: the engine did the waiting.
3. Each `SpawnActor` built a brand-new orb, ran its constructor and
   `BeginPlay` (where it registered with the GameMode), and placed it in the
   world. Then `SetKind` picked a random kind and resized it.
4. After the last wave, `ClearTimer` stopped the timer.

> **A good place to stop.** Everything works now. If you are out of time,
> come back later for "Break it" and "Try it yourself".

## Break it

Something you set in the editor can always be missing. Find out what that
looks like.

1. Select the spawner in the level and set **Orb Class** back to **None**.
2. Save and Play. No orbs appear, and the Output Log shows **your** message,
   in red:

```
LogTemp: Error: OrbSpawner_1: Orb Class is empty. Select the spawner and pick an orb class in the Details panel.
```

It names the Actor, the field, and the fix. Now see what you would get
without that check:

3. Close the editor. In `OrbSpawner.cpp`, comment out the whole
   `if (!OrbClass) { ... }` block. Build, then open the editor (**Orb Class**
   is still **None**), save, and Play.
4. Still no orbs. Search the Output Log for `SpawnActor`. You should find a
   much vaguer warning from the engine, repeated for each orb it was asked to
   make, saying something like:

```
LogSpawn: Warning: SpawnActor failed because no class was specified
```

It does not say which Actor, which field, or what to do. If you saw nothing
at all, that is even worse. Put your check back.

Lesson: **when your code depends on editor settings, check them early and
write the error you would want to read.**

## Try it yourself

1. Change **Seconds Between Waves** to `3` and **Orbs Per Wave** to `10` in
   the spawner's Details panel. No rebuild needed.
2. Give spawned orbs a life span: right after `SetKind`, call
   `Orb->SetLifeSpan(8.f);`. Orbs you do not collect in time vanish by
   themselves. (What happens to `OrbsLeft` then? Think about it before the
   capstone.)
3. Place two spawners with different settings: one fast with few orbs, one
   slow with many.

## Going further

A timer can also call a lambda (lesson 16) instead of a member function.
Look up `FTimerDelegate::CreateLambda` in the Unreal documentation; it lets
you write a short "do this later" without adding a new function.

**Part 1 link:** 16 (passing functions as values), 18 (a spawner that
creates objects at runtime).

Next: [30-enemy-chase](../30-enemy-chase/) - something starts chasing you.
