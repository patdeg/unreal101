# 20 - Your first Actor

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Write your own C++ Actor class, place it in the level, and watch it spin.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a new class:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **An Actor is anything you can place in a level.** Your orb is a class
  `AOrb` that inherits from Unreal's `AActor`, just like lesson 15's `Goblin`
  inherited from `Monster`. You get hundreds of features for free (position,
  rotation, saving in the level) and override only what you need.
- **`Tick(float DeltaTime)` is the game loop.** In lesson 03 you wrote the
  `while` loop yourself. Here the engine runs the loop and calls your `Tick`
  every frame. In JavaScript this is `requestAnimationFrame(callback)`, which
  also hands you the time since the last frame.
- **Unreal adds markers on top of C++.** `UCLASS()` and `GENERATED_BODY()`
  are not C++ features. Before the compiler runs, a tool called **UHT** (the
  Unreal Header Tool) reads your header, finds these markers, and writes
  extra code into `Orb.generated.h`. That code is what lets the editor see
  your class.
- **Every class is still a `.h` and `.cpp` pair**, exactly as in lesson 14.

> **What you can ignore for now**
>
> - `ORBARENA_API`: a word the wizard adds so the class could be used from
>   other modules. Leave it.
> - `RootComponent = CreateDefaultSubobject...` in the constructor. It gives
>   the orb an invisible "point" in the world. Lesson 22 explains it.
> - `DrawDebugBox`: a quick way to see something before the orb has a mesh.
> - `PrimaryActorTick`: just know `bCanEverTick = true` turns Tick on.

## Try it

**1. Create the class with the wizard.** In the editor, choose **Tools > New
C++ Class**. Pick **Actor** as the parent class, click **Next**, name it
**`Orb`**, and keep the suggested path (`Source/OrbArena/`). Click **Create
Class**.

Unreal writes `Orb.h` and `Orb.cpp` and may try to compile them right away.
If it asks you to build from your IDE, or shows a message about Live Coding,
that is fine. If Visual Studio is open and asks to reload the solution
because files changed, click **Reload All**.

**2. Close the editor.** A new class means a full build.

**3. Write the code.** In Visual Studio, find `Orb.h` and `Orb.cpp` in
Solution Explorer (if you do not see them, close Visual Studio, right-click
`OrbArena.uproject`, choose **Generate Visual Studio project files**, and
reopen the `.sln`). Compare the wizard's `Orb.h` with this folder's
`Source/OrbArena/Orb.h`: they should be almost the same. Replace `Orb.cpp`
with this folder's version, or type in the differences. The heart of it:

```cpp
void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DegreesPerSecond = 90.f;
	AddActorLocalRotation(FRotator(0.f, DegreesPerSecond * DeltaTime, 0.f));

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(40.f, 40.f, 40.f),
		GetActorQuat(), FColor::Yellow);
}
```

**4. Build** the **OrbArena** project in Visual Studio, then open the editor.

**5. Place an orb.** In the Content Browser, open **C++ Classes > OrbArena**.
You will find **Orb** there. Drag it into the level, in front of the player
start, about 1 meter off the ground.

Do not worry if you see nothing but the colored move arrows. The orb has
no shape yet: the yellow box is drawn by `Tick`, and `Tick` runs only during
Play. If you click away and lose it, find **Orb** in the **Outliner** panel
(top right, the list of everything in the level) and click it there.

**6. Save and Play.** You should see a yellow wireframe box, turning slowly
like a top, and in the Output Log:

```
LogTemp: Warning: An orb is ready: Orb_1
```

(The number at the end may differ. Unreal names each placed copy.)

Drag in two more orbs. Each one is a separate **object** of the same
**class**, each with its own position, just like lesson 11's fighters.

## What just happened

1. The wizard wrote a class `AOrb : public AActor` with `UCLASS()` and
   `GENERATED_BODY()`.
2. When you built, **UHT ran first**. It read `Orb.h`, saw `UCLASS()`, and
   generated `Orb.generated.h` (in `Intermediate/`, so you never edit it).
   Then the normal C++ compiler built everything.
3. Every frame, the engine called `Tick` on every orb. Your code turned the
   orb by `90 * DeltaTime` degrees, so it spins at 90 degrees per second
   whatever your frame rate is.
4. `Super::Tick(DeltaTime)` let `AActor` do its own per-frame work first.

## Break it

Unreal adds its own rules on top of C++. Here is one, broken on purpose.

1. Close the editor.
2. In `Orb.h`, move the line `#include "Orb.generated.h"` **up**, above
   `#include "GameFramework/Actor.h"`.
3. Build.

This time the error does not come from the C++ compiler at all. It comes
from **UHT**, which runs before the compiler. In the **Output** window (Build
output), look for the first error. It mentions `Orb.h` and says something like:

```
Orb.h(...): error : #include found after .generated.h file - the .generated.h file should always be the last #include in a header
```

Read it calmly. The **where** is still a file and line. The **what** is in
plain English. And there is no `C` number, because it is not from MSVC. That
is your clue that the problem is an Unreal rule, not a C++ rule.

Move the include back to the bottom and build again.

## Try it yourself

1. Change `DegreesPerSecond` to `360.f`. That is only a function body
   change, so try **Live Coding** with the editor open (**Ctrl+Alt+F11**).
2. Make the orb spin on a different axis: put the number in the first slot
   of the `FRotator` (Pitch) instead of the second (Yaw).
3. Make the box bob up and down: in `Tick`, read the location with
   `FVector P = GetActorLocation();`, add a little to `P.Z`, and call
   `SetActorLocation(P);`. (In Unreal, Z is up.)

## Going further

Remove `* DeltaTime` and spin by a fixed `1.5f` degrees per frame instead.
Then, during Play, press the **`` ` ``** key (backtick, under Esc) to open
the console and type `t.MaxFPS 30`. The orb slows down (to half speed if you
were getting 60 frames per second), because its speed now depends on how
many frames you get. That is why `DeltaTime` exists.
(Type `t.MaxFPS 0` to remove the limit.)

**Part 1 link:** 03 (the game loop), 11 (classes), 14 (`.h` and `.cpp`), 15
(inheritance and `override`).

Next: [21-uproperty](../21-uproperty/) - turn the spin speed into a knob you
can change in the editor.
