# 24 - Something touched me

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Collect an orb by walking into it: it notices the player and disappears.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a component and a `UFUNCTION`:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **Collision components can raise events.** A `USphereComponent` is an
  invisible ball. Set it to "overlap", and when something enters it, it fires
  an event called `OnComponentBeginOverlap`.
- **You listen with `AddDynamic`.**
  `Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOrb::HandleOverlap);`
  reads as "when the trigger is overlapped, call `HandleOverlap` on this
  orb". It is `addEventListener` with a member function.
- **The handler must be a `UFUNCTION()`.** This kind of event finds your
  function **by name** when it fires, and only `UFUNCTION`s have a name
  Unreal can look up.
- **`Cast<ACharacter>(OtherActor)`** checks who touched you. It returns a
  pointer to the character, or `nullptr` if `OtherActor` is something else.
  In JavaScript you would write `if (other instanceof Character)`. Lesson
  13's rule applies: check before you use.

> **What you can ignore for now**
>
> - Five of the six handler parameters. Copy the list exactly; use only
>   `OtherActor` today.
> - Collision profiles and channels. `OverlapAllDynamic` is a ready-made
>   preset: "block nothing, notice things that move".
> - Why the binding goes in `BeginPlay`. It is simply the safe place.

## Try it

1. Close the editor.
2. In `Orb.h`, add the `Trigger` component and the `HandleOverlap`
   declaration with its `UFUNCTION()` marker (see this folder's `Orb.h`).
3. In `Orb.cpp`, create and set up the trigger in the constructor, bind it in
   `BeginPlay`, and write `HandleOverlap` (see this folder's `Orb.cpp`). The
   three key lines:

```cpp
// constructor:
Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
// BeginPlay:
Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOrb::HandleOverlap);
// HandleOverlap:
ACharacter* Character = Cast<ACharacter>(OtherActor);
```

4. Build, then open the editor. Place a few orbs at about waist height,
   where you can walk into them.
5. Save and Play. Walk into an orb. It vanishes, and the log says:

```
LogTemp: Warning: Orby was collected by BP_ThirdPersonCharacter_C_0!
```

(The `_C_0` at the end is Unreal's name for "the first object made from the
Blueprint class". Do not worry about it.)

**See the trigger:** during Play, press the backtick key (`` ` ``) and type
`show collision`. Invisible collision shapes are drawn as lines. Type it
again to turn it off.

## What just happened

1. In the constructor, the orb gained a second component, the trigger,
   attached to the mesh.
2. In `BeginPlay`, the orb registered `HandleOverlap` as a listener on the
   trigger's overlap event.
3. When your character's capsule entered the sphere, the physics system
   fired the event. Unreal looked up `HandleOverlap` by name (possible
   because it is a `UFUNCTION`) and called it, passing who touched it.
4. `Cast<ACharacter>` turned the general `AActor*` into an `ACharacter*`. It
   was not `nullptr`, so the orb logged and destroyed itself.

## Break it

Here is a bug that does **not** stop the build. Those are the ones to learn
to spot.

1. Close the editor.
2. In `Orb.h`, delete (or comment out) the `UFUNCTION()` line above
   `HandleOverlap`.
3. Build. **It builds fine.** C++ sees nothing wrong.
4. Open the editor, save, Play, and walk into an orb.

> **If you started the editor from Visual Studio (F5):** when you press
> Play, Visual Studio will probably jump to the front and **stop**, pointing
> at engine code, and the game freezes. You did not crash anything. Unreal
> hit one of its own internal checks (called an "ensure") and the debugger
> paused to show you. Press **F5** (Continue) in Visual Studio and the game
> goes on.

Nothing happens. The orb ignores you. Look carefully at the Output Log after
pressing Play, and search for `Ensure` or `bind`. You should find a message
like this one, often with a long call stack under it and a short hitch when
it appears:

```
Ensure condition failed: ... Unable to bind delegate to 'HandleOverlap' (function might not be marked as a UFUNCTION or object may be pending kill)
```

Read it: Unreal tried to find a function named `HandleOverlap` on the orb,
and because it is not a `UFUNCTION`, it was not on Unreal's list, so the
event was never connected. (The exact wording may differ in your engine
version. If you find no message at all, that is a lesson too: some Unreal
mistakes are completely silent.) A breakpoint in `HandleOverlap`, never hit,
proves the event never reaches your code.

Lesson: **Unreal's event system can only call functions it knows about.**
The `UFUNCTION()` marker is how it knows. Put it back and rebuild.

## Try it yourself

1. Make the trigger bigger with `SetSphereRadius(200.f)` (a constructor
   change: close and rebuild). Now you collect from a distance, like a
   magnet.
2. Log *what* touched the orb even when it is not a character: move the
   `UE_LOG` above the `if`, using `*OtherActor->GetName()`.
3. Instead of `Destroy()`, call `SetActorHiddenInGame(true);` and
   `SetActorEnableCollision(false);`. The orb vanishes but still exists. Can
   you think of when that would be useful?

## Going further

- Read the handler's parameter list as a lesson 10 exercise. Which are
  pointers, which is a `const&`, and why would `FHitResult` be passed by
  `const&` instead of a copy?
- There is also `OnComponentEndOverlap`, fired when something leaves.
- Collision **channels** and **responses** (Block, Overlap, Ignore) decide
  what happens between two objects. Select an orb, click its **Trigger**
  component in the Details panel, and set **Collision > Collision Presets**
  to **Custom** to see the full table.

**Part 1 link:** 10 (`const&`), 13 (pointers and `nullptr` checks), 15 (a
base pointer to a derived object).

Next: [25-game-mode](../25-game-mode/) - keep a score in one place.
