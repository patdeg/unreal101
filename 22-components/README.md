# 22 - Actors are built from parts

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Give the orb a real, visible sphere mesh by building it from a component.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson changes `Orb.h` and the constructor:** close and rebuild.
> (Constructor changes are not safe for Live Coding either.)
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **An Actor is a box of parts called components.** A mesh to see, a light
  to glow, a sphere to detect touches, a camera. Instead of one giant class
  that does everything, you snap parts together. Web developers do the same
  thing when they build a page from small child components.
- **You create components in the constructor** with
  `CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))`. The `<...>`
  is a template argument, the same thing you used in `std::vector<int>`: you
  are saying *what kind* of component.
- **One component is the root.** The root holds the Actor's position and
  rotation. Other components **attach** to it, so they move with it.
- **You never `delete` components.** Unreal's garbage collector owns them.
  That is a big change from lesson 18's `unique_ptr`. Lesson 28 explains how
  it works.

> **What you can ignore for now**
>
> - `ConstructorHelpers::FObjectFinder`. It finds an asset by its path, and
>   only works in a constructor. Copy the pattern.
> - `TObjectPtr<T>`: read it as `T*`. It is the form Unreal recommends for
>   pointers stored in a class.
> - Collision settings. Lesson 24 is about collision.

## Try it

1. Close the editor.
2. In `Orb.h`, add a forward declaration above the class and the `Mesh`
   member inside it:

```cpp
class UStaticMeshComponent;   // above UCLASS(): "this type exists"

// inside the class, under protected:
UPROPERTY(VisibleAnywhere, Category = "Orb")
TObjectPtr<UStaticMeshComponent> Mesh;
```

3. In `Orb.cpp`, add the includes and the constructor code from this
   folder's `Orb.cpp`. The core:

```cpp
Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
RootComponent = Mesh;
```

Remove the lesson 20 `Root` line and the `DrawDebugBox` call: the mesh
replaces both.

4. Build the **OrbArena** project, then open the editor. Your placed orbs are
   now **white spheres**, spinning. (A sphere spinning around its own up axis
   is hard to see. Try the first "Try it yourself" to make it obvious.)

   If your old orbs moved to a strange place (such as the center of the
   map) or look wrong, delete them and drag in fresh ones. A placed Actor
   saves its position on its root component, and you just replaced the root,
   so old copies can lose their position. New copies are fine.
5. Click an orb. At the top of the **Details** panel you now see its
   components: the orb itself and **Mesh** below it. Click **Mesh** to see its
   own settings: **Static Mesh**, **Materials**, and more.

## What just happened

1. In the constructor, the orb created a `UStaticMeshComponent` and made it
   the root.
2. It loaded the engine's built-in sphere and gave it to the mesh component.
3. The orb's `Tick` rotates the Actor, which rotates its root, which is the
   mesh. Any component attached to the root would turn with it.

The header only says `class UStaticMeshComponent;`. That is lesson 14's
forward declaration: the header stores just a pointer, so it only needs to
know the name exists. The `.cpp` includes the full class because it calls
its functions. Unreal code uses this pattern everywhere.

## Break it

This is the most common Unreal compile error of all, so meet it now, on
purpose.

1. Close the editor.
2. In `Orb.cpp`, delete (or comment out) the line
   `#include "Components/StaticMeshComponent.h"`.
3. Build.

The first error in the Output window points at `Orb.cpp` and says something
like:

```
Orb.cpp(...): error C2027: use of undefined type 'UStaticMeshComponent'
```

Often followed by more errors on the next lines. Read only the first.
"Undefined type" means: *the compiler knows this name exists (thanks to the
forward declaration), but it has never seen the class itself, so it cannot
call its functions.* The fix is almost always a missing `#include`. To find
the right file, search the type name in Unreal's online API reference: each
class page shows its header, such as `Components/StaticMeshComponent.h`.

Put the include back and build again.

(If your build still succeeds without that line, some other header you
include happened to include it for you. That is luck, not a guarantee.
Always include what you use.)

## Try it yourself

1. Make the spin visible: in the constructor, change the scale to
   `FVector(0.5f, 0.15f, 0.5f)`, a coin standing on its edge. (A flat disk
   like `FVector(0.5f, 0.5f, 0.2f)` would not help: spinning around its up
   axis, it looks just as still as the sphere.) This is a constructor
   change, so close and rebuild. Or, without any C++, click the **Mesh** of
   one orb in the editor and set **Static Mesh** to another shape, such as
   `Cube`.
   (In the mesh picker, you may need to enable **Show Engine Content** in
   its settings.) When you are done, put the scale back to
   `FVector(0.5f, 0.5f, 0.5f)`: the next lessons expect the round orb.
2. Click the **Mesh** of one orb and set **Materials > Element 0** to a
   different material. Only that orb changes.
3. Add a second component: a `UPointLightComponent` called `Glow`
   (`#include "Components/PointLightComponent.h"`), created with
   `CreateDefaultSubobject` and attached with `Glow->SetupAttachment(Mesh);`.
   Try leaving out `SetupAttachment` and see where the light ends up, and
   whether it still follows the orb.

## Going further

- A `USphereComponent` is an invisible sphere used for collision. Lesson 24
  adds one so the orb can feel the player.
- You will see `TObjectPtr<UStaticMeshComponent>` in class members and plain
  `UStaticMeshComponent*` in function parameters and local variables. Unreal
  recommends `TObjectPtr` for class members and plain `*` elsewhere.

**Part 1 link:** 13 (pointers), 14 (forward declarations and includes).

Next: [23-input](../23-input/) - make the player dash when you press a key.
