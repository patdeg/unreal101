# Bonus - Line traces

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

Optional. Do it after lesson 33 or during the capstone. About 25 minutes.

## Goal

Add a scanner: whatever orb is in the center of your screen gets a cyan
wire sphere drawn around it.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a class and changes a constructor:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **A line trace shoots an invisible ray** from one point to another and
  tells you the first thing it hits. Games use them for everything: "what am
  I looking at", "can the enemy see me", bullets, laser pointers.
- `GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility,
  Params)` returns `true` if it hit something, and fills an `FHitResult`
  (lesson 10's pass by reference) with the details: `Hit.GetActor()` (what),
  `Hit.ImpactPoint` (where).
- **Collision channels decide what a ray can hit.** A trace on the
  **Visibility** channel stops only at things that **block** Visibility. In
  lesson 22 you turned off the orb mesh's collision, so right now traces go
  straight through orbs. You will fix that on purpose.
- **A component you add to the player.** `UOrbScannerComponent :
  UActorComponent` is a part (lesson 22) with its own tick. You add it to the
  character Blueprint, so the template's character code stays untouched.

In JavaScript, the closest thing is `document.elementFromPoint(x, y)`: "what
is under this point?". A line trace is the 3D version.

> **What you can ignore for now**
>
> - `ELevelTick` and `FActorComponentTickFunction` in `TickComponent`. Pass
>   them on to `Super` and forget them.
> - Trace "complex" collision, multi-traces, sphere traces. One ray is plenty.

## Try it

1. In the editor, choose **Tools > New C++ Class**, pick **Actor
   Component**, and name it **`OrbScannerComponent`**. Then close the
   editor.
2. Replace `OrbScannerComponent.h` and `.cpp` with this folder's versions.
   The heart of it:

```cpp
FHitResult Hit;
const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End,
	ECC_Visibility, Params);
AOrb* Orb = Cast<AOrb>(Hit.GetActor());
```

3. **Do not change `Orb.cpp` yet.** Build, then open the editor.
4. Open `BP_ThirdPersonCharacter`. In the **Components** panel, click
   **+ Add**, search for **Orb Scanner**, and add it. Compile and save.
5. Save (**Ctrl+Shift+S**) and Play. A red line runs from your character to
   whatever is in the center of the screen. (The real ray starts at the
   camera, but a line drawn from the camera straight along your view would
   look like a dot, so the code draws it from the character. See the comment
   in `TickComponent`.) Aim it at an orb.

Nothing is highlighted. Keep reading: that is this lesson's "Break it",
arriving by itself.

## Break it (it already is)

The scan line goes **through** the orb and stops at the floor or wall behind
it. Make the code tell you what it hit. In `TickComponent`, right after the
`if (!bHit) return;`, add:

```cpp
UE_LOG(LogTemp, Log, TEXT("Scan hit: %s"), *Hit.GetActor()->GetName());
```

Live Coding is enough (only a function body changed). Play, aim at an orb,
and read the Output Log:

```
LogTemp: Scan hit: Floor
```

(or a wall, or the landscape: something behind the orb). The trace is
working perfectly. It is the **orb** that does not block the Visibility
channel: in lesson 22 you set its mesh to `NoCollision` so the player could
walk through it. A ray cannot hit what has no collision.

Is `Hit.GetActor()` always safe to use with `->` there? Not quite. Even when
`bHit` is true, a hit on some kinds of geometry has no Actor, and then
`GetActor()` returns `nullptr`. Lesson 13 says check it. Change the log line
to check `Hit.GetActor() != nullptr` first. Then remove the log line when you
are done, because it prints every frame.

**Fix it:** in `Orb.cpp`, replace the `NoCollision` line with the three
lines in this folder's `Orb.additions.txt`. They say: "ignore everything,
except block Visibility traces". The player still walks through orbs, but
the scanner can see them. This is a constructor change, so close and
rebuild, then Play. Aim at an orb: a cyan sphere surrounds it.

## What just happened

1. Every frame, the scanner asked the player controller where the camera
   is and where it points.
2. It traced a ray from there, ignoring the player's own body.
3. The ray stopped at the first thing that blocks the Visibility channel. At
   first that was never an orb (no collision), so you saw the name of the
   floor or wall behind it.
4. After the fix, the orb's mesh blocks Visibility (and nothing else), the
   ray stops at it, `Cast<AOrb>` succeeds, and the scanner draws the
   highlight.

## Try it yourself

1. Remove `Params.AddIgnoredActor(OwnerPawn);` and see whether the ray now
   hits your own character first. (It depends on the character's collision
   settings, which is exactly why ignoring yourself is a good habit.)
2. Show the orb's kind on screen while you look at it (a public `GetKind()`
   on `AOrb`, and `GEngine->AddOnScreenDebugMessage` from lesson 31).
3. Make it a "tractor beam": while you look at an orb and hold a key, pull
   it toward you with `SetActorLocation`.

## Going further

`LineTraceMultiByChannel` returns **every** hit along the ray, not just the
first. `SweepSingleByChannel` moves a whole sphere along the line instead of
a thin ray: handy for "is there room to dash?".

**Part 1 link:** 10 (output parameters by reference), 13 (checking
pointers).

Back to: [34-capstone](../34-capstone/) | Also optional:
[bonus-save-game](../bonus-save-game/)
