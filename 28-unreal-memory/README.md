# 28 - Memory the Unreal way

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Build a tracker that points at the nearest orb, and keeps working safely when
that orb is collected and destroyed.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a new class:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

This is the Part 2 version of lesson 13: the hard one. It is normal to read
it twice. The good news: Unreal does more of the work for you than plain C++
did, once you know its rules.

## The new idea

In lessons 17 and 18 you learned plain C++'s answer to "who deletes this?":
an owner, such as a `unique_ptr`, deletes the object when the owner goes
away. **Unreal takes the other road: a garbage collector**, like
JavaScript's. Every so often, Unreal looks at which objects are still
reachable and frees the rest. You never write `delete` for an Unreal object.

There is one catch, and it is the whole lesson: **Unreal's garbage collector
only sees pointers it knows about.**

| Kind of pointer | Keeps the object alive? | When the object is destroyed | Use it for |
|---|---|---|---|
| `UPROPERTY() TObjectPtr<AOrb> Orb;` | Yes (until something calls `Destroy()` on it) | Unreal knows about it, so it can set it to null later, when the garbage collector runs. Until then, check with `IsValid` | Things you own or rely on (components, lesson 22) |
| `TWeakObjectPtr<AOrb> Orb;` | No | `Orb.IsValid()` becomes false | Things that can vanish at any time (today's tracker) |
| `AOrb* Orb;` as a class member, no `UPROPERTY` | No | **Nothing tells it.** It still holds the old address | Avoid as a member. Fine as a local variable inside one function |

And one check for any Unreal object pointer: **`IsValid(Ptr)`**. It is true
only if the pointer is not null **and** the object is not being destroyed.
`Ptr != nullptr` checks only the first half.

| Part 1 rule | Unreal rule |
|---|---|
| The owner (`unique_ptr`) deletes the object | The garbage collector frees it when nothing known holds it |
| A plain pointer observes; it can dangle | A plain member pointer can dangle too, so use `TWeakObjectPtr` to observe |
| Check `if (ptr)` | Check `IsValid(Ptr)` (or `Weak.IsValid()`) |
| `delete` / end of scope | `Destroy()` for Actors; never `delete` |

> **What you can ignore for now**
>
> - When exactly the garbage collector runs. "Every so often" is enough.
> - `TNumericLimits<double>::Max()`: just "a very big number to start the
>   search with".
> - `TStrongObjectPtr`, `TSharedPtr`, `TUniquePtr`: other smart pointers for
>   special cases.

## Try it

1. In the editor, choose **Tools > New C++ Class**, pick **Actor**, and name
   it **`OrbTracker`**. Then close the editor.
2. Replace `OrbTracker.h` and `OrbTracker.cpp` with this folder's versions.
   The heart of it:

```cpp
// OrbTracker.h
TWeakObjectPtr<AOrb> Target;

// OrbTracker.cpp, in Tick
if (!Target.IsValid())
{
	Target = FindNearestOrb();
	if (!Target.IsValid())
	{
		return;
	}
}
AOrb* Orb = Target.Get();
DrawDebugLine(GetWorld(), GetActorLocation(), Orb->GetActorLocation(),
	FColor::Green, false, -1.f, 0, 3.f);
```

3. Build, then open the editor. Drag an **OrbTracker** (from **C++ Classes >
   OrbArena**) into the level, somewhere you can see it. It is invisible;
   the line is what you will see. (Like the lesson 20 orb, you only see its
   move arrows while it is selected. To find it again, click
   **OrbTracker** in the **Outliner**.)
4. Save and Play. A green line runs from the tracker to the nearest orb.
   Collect that orb. The line instantly jumps to the next nearest one. Keep
   going until none are left: no crash, the line just disappears.

## What just happened

1. The tracker kept a **weak** pointer to an orb. It did not keep the orb
   alive, and it did not pretend to own it.
2. When you touched the orb, it called `Destroy()`. Unreal marked it as
   "being destroyed" right away, even though the memory is freed later.
3. On the next frame, `Target.IsValid()` saw that mark and returned false.
   The tracker searched again.
4. `FindNearestOrb` used `RemoveAll` with a lambda (lesson 16) to drop any
   orb that was not `IsValid`, then found the closest one with a plain loop.

## Break it

Now see exactly why a plain pointer is not good enough. This version
shows you the problem in the log, and then cleans up after itself so it
does not crash.

1. Close the editor. In `OrbTracker.h`, add a plain pointer next to the
   weak one:

```cpp
AOrb* RawTarget = nullptr;   // a plain pointer, no UPROPERTY: the bad kind
```

2. In `Tick`, right after `AOrb* Orb = Target.Get();`, add:

```cpp
RawTarget = Orb;
```

3. And at the very top of `Tick` (after `Super::Tick`), add a report:

```cpp
if (RawTarget != nullptr && !IsValid(RawTarget))
{
	UE_LOG(LogTemp, Error, TEXT("RawTarget is not null, but IsValid says it is dead!"));
	RawTarget = nullptr;   // forget it, so we report it only once
}
```

4. Build, then open the editor, save, Play, and collect the orb the line
   points to.

On the next frame, a red line appears, exactly one per collected orb (even
the last one):

```
LogTemp: Error: RawTarget is not null, but IsValid says it is dead!
```

Read that carefully. The plain pointer still holds an address, so
`!= nullptr` says "fine". But the orb is destroyed. If the tracker trusted
`RawTarget` and called `RawTarget->GetActorLocation()`, it would be using a
dead object. For now the memory is still there, so it might even seem to
work. Later, when the garbage collector frees that memory, the same line
reads freed memory: lesson 13's dangling pointer, and the kind of crash that
happens "randomly" a minute after the real mistake.

That is also why the report sets `RawTarget = nullptr`. Without that line,
after the **last** orb the tracker has no new target, so `RawTarget` would
keep the dead address forever. The red line would print every frame, and
once the garbage collector freed the orb (often within a minute), even the
`IsValid(RawTarget)` check itself would be reading freed memory, and could
crash. `IsValid` can only tell you about an object whose memory still
exists. A plain pointer you never clear is a time bomb.

Lesson: **for Unreal objects, `IsValid` is the real check, and a plain
member pointer is a trap.** Remove the three `RawTarget` additions.

## Try it yourself

1. Make the tracker visible: give it a small cube mesh component, the way
   lesson 22 gave the orb a sphere.
2. Draw the line in a different color for Golden orbs. (The tracker will
   need to read the orb's kind; add a public `GetKind()` getter to `AOrb`.)
3. Switch to a closer orb. Instead of searching only when the target dies,
   also search every 2 seconds, so the line moves when a closer orb exists.
   Keep a `float TimeSinceSearch` and add `DeltaTime` to it each frame.

## Going further

- `check(Ptr)` stops the program with a message if `Ptr` is null: a loud
  crash on purpose, for "this can never happen" bugs. `ensure(Ptr)` reports
  the problem once and keeps running.
- For plain C++ helper objects that are **not** Unreal objects (no `U` or
  `A` prefix), Unreal has `TUniquePtr` and `TSharedPtr`, the same ideas as
  lesson 18's `std::unique_ptr` and `std::shared_ptr`.

**Part 1 link:** 13 (dangling pointers), 16 (lambdas), 17 (destructors), 18
(ownership).

Next: [29-spawning-and-timers](../29-spawning-and-timers/) - make orbs appear
in waves.
