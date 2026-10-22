# 30 - Something chases you

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Add an enemy that walks toward the player wherever they go, and says
"Caught!" when it gets close.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a new class:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **Direction is subtraction.** To go from where you are to where the player
  is: `Player - Me`. If you ever made a 2D chase game in JavaScript, you
  wrote `dx = player.x - me.x; dy = player.y - me.y;` and then divided by the
  length so the speed stayed the same. It is the same here, with three
  numbers (X, Y, Z) packed into one `FVector`, and `-` works on the whole
  vector at once:

```cpp
FVector Direction = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
```

  `GetSafeNormal()` is "divide by the length": it keeps the direction and
  makes the length exactly 1.
- **A Character walks for you.** `AEnemy` inherits from `ACharacter`, the
  same parent as the player. Call `AddMovementInput(Direction)` every frame
  and the movement component handles speed, gravity, slopes and stairs.
- **Someone must be in control.** A Character only moves when a controller
  possesses it: the player's, or an AI controller. `AutoPossessAI =
  EAutoPossessAI::PlacedInWorldOrSpawned;` gives the enemy an AI controller
  automatically.
- **`Tick` is the brain.** Every frame: find the player, compute the
  direction, step. That is lesson 03's game loop again, but per enemy.

> **What you can ignore for now**
>
> - The AI controller's real powers (path finding, behavior trees). We only
>   need it to exist.
> - `bOrientRotationToMovement`: makes the enemy turn to face where it
>   walks.
> - Why the player's speed differs from the enemy's. Both are just numbers on
>   the movement component.

## Try it

1. In the editor, choose **Tools > New C++ Class**. This time pick
   **Character** as the parent, and name it **`Enemy`**. Then close the
   editor.
2. Replace `Enemy.h` and `Enemy.cpp` with this folder's versions.
3. Build, then open the editor. Drag an **Enemy** (from **C++ Classes >
   OrbArena**) into the level, some distance away from the player start.
   Make sure it stands on the floor.
4. Save and Play. A tall grey block turns toward you and walks after you.
   Let it reach you:

```
LogTemp: Warning: Caught! Enemy_1 got you.
```

Now dash away (lesson 23). Can you outrun it?

**Wait, something is wrong.** Place an orb between the enemy and you, and
watch. The **enemy collects the orb**, and you get the points! Lesson 24's
check was `Cast<ACharacter>(OtherActor)`, and the enemy *is* an
`ACharacter`. Fix it in `Orb.cpp`: cast to the **player's** class instead.

```cpp
#include "OrbArenaCharacter.h"
// ...
AOrbArenaCharacter* Player = Cast<AOrbArenaCharacter>(OtherActor);
if (Player == nullptr)
{
	return;
}
```

Only `Orb.cpp` changed, so Live Coding is enough (close and rebuild works
too). Now only the player collects.

## What just happened

1. When the level started, the enemy was given an AI controller, so it was
   allowed to move.
2. Every frame, its `Tick` found the player with
   `UGameplayStatics::GetPlayerPawn(this, 0)` and computed an arrow from
   itself to the player.
3. It flattened the arrow (Z = 0), shrank it to length 1, and passed it to
   `AddMovementInput`. The movement component turned that wish into actual
   walking.
4. `ToPlayer.Size()` measured the distance; under 150 cm meant "caught".

## Break it

Two breaks today: one the compiler catches, one it never could.

**Part 1: a missing include.** Close the editor. In `Enemy.cpp`, comment out
`#include "Kismet/GameplayStatics.h"`. Build. The first error is something
like:

```
Enemy.cpp(...): error C2653: 'UGameplayStatics': is not a class or namespace name
```

Compare with lesson 22's C2027 "use of undefined type": there, the compiler
knew the **name** but not the class. Here it does not even know the name.
Both have the same fix: find the right header (search the class name in
Unreal's API reference) and include it. Put it back.

(If your build still succeeds without that line, that is lesson 22's luck
again. To build faster, Unreal sometimes glues several `.cpp` files of your
project into one big file before compiling, and by now `OrbTracker.cpp`
includes `GameplayStatics.h`. If it lands in the same glued file before
`Enemy.cpp`, the include "leaks" in. That is
luck, not a guarantee: always include what you use.)

**Part 2: a bug no compiler can catch.** Swap the subtraction:

```cpp
const FVector ToPlayer = GetActorLocation() - Player->GetActorLocation();   // reversed
```

Only a function body changed, so use Live Coding, then save and Play. It
compiles perfectly, and the enemy **runs away from you**. `A - B` points from
B to A. The compiler checks that you subtract two vectors; it cannot know
which way you meant. When a game does the opposite of what you wanted, check
the order of a subtraction. Swap it back.

## Try it yourself

1. Make the enemy faster than the player: select it and set **Walk Speed**
   to `700` in the Details panel. Then slower: `200`.
2. Place three enemies. Each runs its own `Tick` with its own state, like
   three objects of lesson 11's `Fighter` class.
3. Make the enemy only chase when the player is within 1500 cm: check
   `ToPlayer.Size()` before calling `AddMovementInput`. (The capstone turns
   this into proper enemy states.)

## Going further

- Looking the player up every frame works, but you could look once and
  store it. Use lesson 28's `TWeakObjectPtr<APawn>`, not a plain pointer:
  the player can be destroyed too.
- Unreal has real path finding: a **Nav Mesh Bounds Volume** in the level
  plus an AI controller's `MoveToActor`. Then enemies walk around walls
  instead of into them.

**Part 1 link:** 03 (the game loop), 11 (each object has its own state), 15
(inheritance: the enemy and the player share a parent).

Next: [31-delegates](../31-delegates/) - announce score changes to anyone who
wants to listen.
