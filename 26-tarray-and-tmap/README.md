# 26 - Unreal's containers

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Give each orb a type with its own points, count the orbs, and declare a win
when the last one is collected.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds `UPROPERTY`s and a function to two headers:** close and
> rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

Unreal has its own versions of the standard containers you met in Part 1.
They work the same way, with slightly different names:

| Part 1 (standard C++) | Unreal | JavaScript |
|---|---|---|
| `std::vector<int>` | `TArray<int32>` | an array |
| `v.push_back(x)` | `A.Add(x)` | `arr.push(x)` |
| `v.size()` | `A.Num()` | `arr.length` |
| `for (auto x : v)` | `for (auto x : A)` | `for (const x of arr)` |
| `std::map<std::string, int>` | `TMap<FName, int32>` | a `Map` |
| `m["key"] = 5;` | `M.Add(TEXT("key"), 5);` | `map.set("key", 5)` |
| `m.find(k)` / `m.contains(k)` | `M.Find(k)` / `M.Contains(k)` | `map.get(k)` / `map.has(k)` |

Why does Unreal not just use `std::vector`? Mostly history and control:
Unreal's containers work with the editor (`UPROPERTY`), with saving, and with
the garbage collector. When you write Unreal code, use the `T` versions.

- **`TMap::Find` returns a pointer.** You get the address of the value, or
  `nullptr` if the key is missing. Remember lesson 07's trap, where
  `prices["sowrd"]` silently added a typo to the map? `Find` never adds
  anything. It answers "here it is" or "not found", and you check which.

> **What you can ignore for now**
>
> - `UGameplayStatics`: a big toolbox class of handy functions ("Kismet" in
>   its path is an old name for Blueprints). You use one tool from it today.
> - `AOrb::StaticClass()`: "the class `AOrb` as a value". It is how you tell
>   a function which class you mean.
> - `UE_LOG` with `Log` verbosity: the same as `Warning`, but grey and
>   quieter.

## Try it

1. Close the editor.
2. **GameMode:** add `OrbCollected`, `PointsTable` and `OrbsLeft` to the
   header, then the code from this folder's `OrbArenaGameMode.cpp`: fill the
   table in the constructor, count the orbs in `BeginPlay`, and write
   `OrbCollected`. The two container moments:

```cpp
// a TArray, filled for us:
TArray<AActor*> FoundOrbs;
UGameplayStatics::GetAllActorsOfClass(this, AOrb::StaticClass(), FoundOrbs);
OrbsLeft = FoundOrbs.Num();

// a TMap lookup that can fail:
const int32* Points = PointsTable.Find(OrbType);
if (Points != nullptr)
{
	AddScore(*Points);
}
```

3. **Orb:** add the `OrbType` property to `Orb.h`, and in `Orb.cpp` call
   `GameMode->OrbCollected(OrbType)` instead of `AddScore(1)`.
4. Build, then open the editor.
5. Place five or six orbs. Select a few and change their **Orb Type** in the
   **Details** panel to `Rare` or `Golden`. Type the spelling exactly. (`FName`
   ignores upper and lower case, so `golden` also works, but it does not
   forgive typos.)
6. Save, Play, and collect them all:

```
LogTemp: Warning: 6 orbs to collect. Go!
LogTemp: Warning: Score: 1
LogTemp: Warning: Score: 21
...
LogTemp: Warning: You win! Final score: 33
```

**Tune the table in the editor.** Open your GameMode Blueprint (the one you
checked in lesson 25), click **Class Defaults**, and find **Points Table**
under **Orbs**. (No Class Defaults button? Then it opened as a data-only
Blueprint, and the list you see already is the Class Defaults: see lesson
23, step 7.) You can change the points, or add a new row, without touching
C++.

## What just happened

1. At the start, the GameMode asked for every `AOrb` in the level. It got a
   `TArray` and counted it with `Num()`.
2. Each orb, when touched, told the GameMode its **type**, not its points.
3. The GameMode looked the type up in its `TMap` with `Find`. A pointer came
   back. It checked the pointer, then read the value with `*`.
4. When the count reached zero, the referee declared a win.

The orb knows only its type. The referee knows the rules. Changing how much
a Golden orb is worth is one edit, in one place.

## Break it

Lesson 13's rule, with a new container. **Save first**: this one crashes the
editor.

1. Close the editor. In `OrbCollected`, replace the safe lookup with one
   that trusts `Find` blindly:

```cpp
const int32 Value = *PointsTable.Find(OrbType);   // no check!
AddScore(Value);
```

2. Build, then open the editor. Set one orb's **Orb Type** to `Legendary`,
   a type that is not in the table.
3. Save (**Ctrl+Shift+S**), Play, and collect that orb.

The Crash Reporter shows something like this (or, if you started the editor
with **F5**, Visual Studio stops on the line instead):

```
Unhandled Exception: EXCEPTION_ACCESS_VIOLATION reading address 0x0000000000000000
```

This time the address is **exactly zero**: `Find` returned `nullptr`, and
the `*` tried to read the value stored at address 0. The call stack shows
`AOrbArenaGameMode::OrbCollected` near the top. Same crash as lesson 13,
same fix: put the `if (Points != nullptr)` back. Build, reopen the editor,
and play again: your red error message now names the missing type instead.

As in lesson 25: close the Crash Reporter with **Close Without Sending**,
and if Unreal offers to restore unsaved packages when you reopen, click
**Skip**.

Lesson: **a lookup that can fail returns something you must check.** Unreal
chose a pointer for `Find` precisely so that you would have to think about
it.

## Try it yourself

1. Add a `Legendary` row to the **Points Table** in the GameMode Blueprint,
   worth 100. Collect the Legendary orb again. No C++ needed.
2. Keep a `TArray<FName> Collected;` in the GameMode and `Add` each type as
   it is collected. At the win, loop over it and log the whole list.
3. Use `PointsTable.Contains(OrbType)` instead of `Find` in an extra check
   at `BeginPlay`: loop over the found orbs and warn about any orb whose
   type is not in the table, **before** the player collects it. (You will
   need to cast each `AActor*` to `AOrb*`, and give `AOrb` a public
   `GetOrbType()` getter.)

## Going further

`TArray` has a `Sort` that takes a lambda, like lesson 16's `std::sort`:
`Scores.Sort([](int32 A, int32 B) { return A > B; });`. And `RemoveAll`
takes a lambda that says which items to drop. You will use it in lesson 28.

**Part 1 link:** 05 (`std::vector`), 07 (`std::map` and its trap), 13
(checking pointers).

Next: [27-uenum-and-ustruct](../27-uenum-and-ustruct/) - replace typed-in
names with a real type.
