# 21 - Knobs in the editor

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Give every orb its own spin speed and nickname, set in the editor, without
recompiling.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds `UPROPERTY`s:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **`UPROPERTY()` makes a member variable visible to the editor.** In lesson
  08 a struct member was just a variable. Put `UPROPERTY(EditAnywhere)` above
  it and it shows up in the **Details** panel, where you can change it with
  the mouse.
- **The C++ value is only the default.** `float RotationSpeed = 90.f;` is
  what a new orb starts with. Each placed orb can then have its own value,
  saved in the level. This is called **data-driven**: you write the code
  once, then tune numbers without touching it. In JavaScript terms: like a
  component with props, where the code gives defaults and each use of the
  component can pass its own value.
- **`FString` is Unreal's `std::string`** (lesson 06). Unreal has its own
  versions of most standard library types, with an `F` in front. You still
  get `+` to join text, `.Len()` for the length, and so on.
- **Unreal's numbers have sizes in their names.** You will see `int32` in
  Unreal code instead of `int`. It is the same idea, with the size (32 bits)
  written down so it is the same on every platform.

> **What you can ignore for now**
>
> - The other specifiers you will see in Unreal code (`BlueprintReadWrite`,
>   `VisibleAnywhere`, `meta = (...)`). Lesson 22 and later use a few.
> - The details of why `%s` wants a `*`. The code comment gives the short
>   version. Just remember the rule: an `FString` going into `%s` gets a `*`
>   in front.

## Try it

1. Close the editor.
2. Add the two `UPROPERTY` members from this folder's `Orb.h`:

```cpp
UPROPERTY(EditAnywhere, Category = "Orb")
float RotationSpeed = 90.f;

UPROPERTY(EditAnywhere, Category = "Orb")
FString Nickname = TEXT("Orby");
```

3. In `Orb.cpp`, use `RotationSpeed` in `Tick` instead of the fixed `90.f`,
   and log both values in `BeginPlay` (see this folder's `Orb.cpp`).
4. Build the **OrbArena** project in Visual Studio, then open the editor.
5. Click one of your placed orbs. In the **Details** panel (on the right),
   find the **Orb** section. Set **Rotation Speed** to `20` on one orb and
   `400` on another. Give them nicknames.

Unreal shows `RotationSpeed` as "Rotation Speed": it splits the name at the
capital letters for you.

6. Save and Play. The orbs spin at different speeds, and the log says:

```
LogTemp: Warning: Orb_1 (Slowpoke) spins at 20.000000 degrees per second
LogTemp: Warning: Orb_2 (Zoomer) spins at 400.000000 degrees per second
```

You changed the game's behavior with no recompiling at all.

## What just happened

1. `UPROPERTY(EditAnywhere)` told UHT to describe `RotationSpeed` to the
   editor: its name, its type (`float`) and its category.
2. The editor used that description to draw a number box in the Details
   panel.
3. When you typed a value, the editor stored it **in the level, for that one
   orb**. When the game started, Unreal created each orb (running the
   constructor and the `= 90.f` default) and then applied the saved value on
   top.

## Break it

UHT checks your markers, and it is fussy about spelling.

1. Close the editor.
2. In `Orb.h`, misspell one specifier: `UPROPERTY(EditAnywhere, Categroy =
   "Orb")`.
3. Build.

The C++ compiler never gets a chance: UHT stops first. In the Output window,
the first error names `Orb.h` and its line, and says something about an
unknown specifier, quoting your misspelled word:

```
Orb.h(...): error : Unknown variable specifier 'Categroy'
```

The exact wording may differ a little, but the **file**, the **line** and
**your typo in quotes** are all there. When an Unreal build fails and the
error has no `C` number, read it as a UHT message: something inside a
`UCLASS`, `UPROPERTY` or `UFUNCTION` marker is wrong.

Fix the spelling and build again.

## Try it yourself

1. **The surprise.** Set an orb's **Rotation Speed** to `20` in the editor and
   save. Then change the C++ default to `float RotationSpeed = 180.f;` and
   close and rebuild. The orb you edited **still spins at 20**. Orbs you never
   touched now spin at 180. A new orb dragged in starts at 180. The editor
   keeps per-orb values separately from the C++ default. A small curved
   "reset" arrow appears next to any value that differs from the default.
   Click it to go back.
2. Add `UPROPERTY(EditAnywhere, Category = "Orb") int32 Points = 1;` and log
   it with `%d`.
3. Add a property called `bClockwise` of type `bool`, and spin the other way
   when it is false (multiply the speed by `-1`). Unreal names `bool`
   members with a `b` at the front, and the editor shows them as a tick box.

## Going further

- `meta = (ClampMin = "0")` inside the `UPROPERTY` stops the editor from
  accepting values below zero.
- `VisibleAnywhere` shows a value but does not let you edit it.
- Unreal has three text types: `FString` (text you change, like
  `std::string`), `FName` (a fast name tag for comparing, used in lesson 26)
  and `FText` (text the player reads, which can be translated).

**Part 1 link:** 02 (types and defaults), 06 (strings), 08 (struct members).

Next: [22-components](../22-components/) - give the orb a real mesh.
