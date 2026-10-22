# 32 - C++ and Blueprints together

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Let a Blueprint add the sparkle (a sound, a burst of particles) when an orb
is collected, while C++ keeps the rules.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a `UFUNCTION` to `Orb.h`:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

You have been using Blueprints all along: the template's character and
GameMode are Blueprint children of C++ classes. Today you make your own.

- **A Blueprint can be a child of your C++ class.** `BP_Orb` inherits from
  `AOrb` exactly the way lesson 15's `Goblin` inherited from `Monster`. It
  gets all your C++ (spinning, collecting, scoring) and can add more, drawn
  as nodes in a graph instead of typed as code.
- **`BlueprintImplementableEvent` is a hook for Blueprints.**
  `UFUNCTION(BlueprintImplementableEvent) void OnCollected();` is declared
  in C++ and **called** in C++, but **implemented** in the Blueprint. It is
  lesson 15's `virtual` function, where the override is a graph.
- **Who does what.** A common split in real studios: **C++ for rules** (what
  counts as a collection, how many points) and **Blueprints for looks and
  feel** (which sound, which particles, how big the flash). Designers can
  change the feel without a C++ rebuild.

| Marker | Who writes the body | Who calls it |
|---|---|---|
| `UFUNCTION(BlueprintCallable)` | C++ | Blueprints |
| `UFUNCTION(BlueprintImplementableEvent)` | the Blueprint | C++ |
| `UFUNCTION(BlueprintNativeEvent)` | C++ gives a default, Blueprint may replace it | C++ |

> **What you can ignore for now**
>
> - Most of the Blueprint editor. You need the **Event Graph**, the
>   right-click node search, and the **Compile** and **Save** buttons.
> - Particle systems and sounds themselves. You only place existing ones.
> - `BlueprintNativeEvent`, in "Going further".

## Try it

**Part A: the C++ hook.**

1. Close the editor.
2. In `Orb.h`, add the event, and add `BlueprintReadOnly` to the `Kind`
   property so Blueprints can read it:

```cpp
UFUNCTION(BlueprintImplementableEvent, Category = "Orb")
void OnCollected();
```

3. In `Orb.cpp`, at the end of `HandleOverlap`, call it right before
   `Destroy()`:

```cpp
OnCollected();
Destroy();
```

Do **not** write a body for `OnCollected` in `Orb.cpp`. That is the "Break
it" below.

4. Build, then open the editor.

**Part B: the Blueprint.**

5. In the Content Browser, open **C++ Classes > OrbArena**. Right-click
   **Orb** and choose **Create Blueprint class based on Orb**. Name it
   **`BP_Orb`** and save it in a folder of your own (for example
   `Content/OrbArena`).
6. Open `BP_Orb`. Go to the **Event Graph** tab. Right-click the empty grid
   and type `On Collected`. Pick **Event On Collected**. A red event node
   appears: that is the Blueprint side of your C++ hook.
7. Drag from its white arrow (the execution pin) and add effects. Choose
   what you have:
   - **No extra content:** add a **Print String** node with the text
     `Sparkle!`.
   - **With Starter Content** (ticked in PREP.md, or added later from
     **Add > Add Feature or Content Pack**): add **Play Sound at Location**
     and pick a sound (for example an explosion sound from the Starter
     Content audio folder), and **Spawn Emitter at Location** with a particle
     system (for example an explosion or fire effect from the Starter
     Content particles folder). For both **Location** pins, add a **Get Actor
     Location** node and connect it.
8. Click **Compile**, then **Save**.

**Part C: use it.**

9. Select your **OrbSpawner** in the level and set **Orb Class** to
   **BP_Orb**. Also replace a few placed orbs with `BP_Orb` (drag it from
   the Content Browser).
10. Save (**Ctrl+Shift+S**), Play, and collect orbs. Each one goes out with
    your effect, and the score still counts, because the C++ rules came along
    through inheritance.

## What just happened

1. UHT saw `BlueprintImplementableEvent` and wrote the body of `OnCollected`
   for you, in `Orb.gen.cpp`. That generated body says, roughly, "find the
   Blueprint's version of this event and run it".
2. `BP_Orb` is a child class of `AOrb`. Spawned and placed `BP_Orb` objects
   run all of `AOrb`'s C++, plus the graph.
3. When the player touched a `BP_Orb`, your C++ `HandleOverlap` ran, scored,
   called `OnCollected()` (the graph ran: sound, particles), then destroyed
   the orb.

## Break it

**Break 1: write a body for the event.** It feels natural to write one, so
see what happens.

1. Close the editor. In `Orb.cpp`, add:

```cpp
void AOrb::OnCollected()
{
	UE_LOG(LogTemp, Warning, TEXT("collected"));
}
```

2. Build. You get an error that the function **already has a body** or is
   **already defined**. Depending on how the build groups your files, it is
   either a compiler error such as:

```
error C2084: function 'void AOrb::OnCollected(void)' already has a body
```

or a linker error (lesson 14) such as:

```
error LNK2005: "public: void __cdecl AOrb::OnCollected(void)" ... already defined in ...Orb.gen.cpp.obj
```

Either way, the clue is the same: **two bodies for one function**, one of
them in a `.gen.cpp` file you never wrote. That file belongs to UHT, which
already wrote the body. Delete yours and build again.

**Break 2: the wrong class in the world.** Set the spawner's **Orb Class**
back to the C++ **Orb** (not `BP_Orb`). Save and Play. Orbs spawn, score
works, but there are no effects, and no error. The C++ class has no graph, so
its `OnCollected` does nothing. Lesson: **know which class is actually in the
world.** Click a spawned orb in the Outliner during Play: its class name tells
you. Set it back to `BP_Orb`.

## Try it yourself

1. In `BP_Orb`, use the **Kind** variable (drag it in with **Get Kind**, or
   search `Get Kind`) and a **Switch on EOrbKind** node to play a different
   effect for Golden orbs. Your C++ enum works in Blueprints because of
   `UENUM(BlueprintType)` (lesson 27).
2. Give `BP_Orb` a different material: in the `BP_Orb` editor, select the
   **Mesh** component in the **Components** panel (top left), then set
   **Materials > Element 0** in the Details panel. Compile and save. Every
   `BP_Orb` changes; C++ orbs do not. (Changing the Mesh's **Scale** there
   will not stick: `ApplyKind` sets the size at Play.)
3. Add a `UFUNCTION(BlueprintCallable) int32 GetPoints() const;` to the orb
   in C++, and call it from the graph to print the orb's points.

## Going further

`UFUNCTION(BlueprintNativeEvent)` is the in-between: C++ gives a default
body, named with an `_Implementation` suffix (`void
OnCollected_Implementation();`), and a Blueprint may replace it. Use it when
the C++ class should work on its own but designers may want to change the
behavior.

**Part 1 link:** 15 (`virtual` functions and child classes).

Next: [33-umg-hud](../33-umg-hud/) - put the score on the screen.
