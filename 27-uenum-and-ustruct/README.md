# 27 - Typed, data-driven orbs

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Replace the typed-in orb names with a real type, and tune every kind of orb
(points and size) from one table in the editor.

**Time:** about 40 to 50 minutes, longer than most lessons. There is a
marked good place to stop after step 8.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a new header and new types:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **`UENUM` is lesson 09's `enum class`, made visible to the editor.**
  `UENUM(BlueprintType) enum class EOrbKind : uint8 { Common, Rare, Golden };`
  The editor shows it as a **dropdown**, so `"Goldne"` can no longer happen.
  The `: uint8` stores it in one byte; Unreal requires that for enums that
  Blueprints can use.
- **`USTRUCT` is lesson 08's `struct`, made visible to the editor.** It
  needs `GENERATED_BODY()` as its first line (like a `UCLASS`), and each
  member the editor should show gets its own `UPROPERTY`.
- **Data-driven design.** The GameMode holds a
  `TMap<EOrbKind, FOrbKindInfo>`: for each kind, a struct of settings. A
  designer (you, wearing a different hat) can change every number in the
  editor without touching C++. In JavaScript terms: the code reads a config
  object, like a JSON file, instead of having numbers baked in.

| Part 1 | Unreal | Why the marker? |
|---|---|---|
| `enum class Stance { ... };` | `UENUM(BlueprintType) enum class EOrbKind : uint8 { ... };` | So the editor can show a dropdown |
| `struct Fighter { int hp = 0; };` | `USTRUCT(BlueprintType) struct FOrbKindInfo { GENERATED_BODY() UPROPERTY(...) int32 Points = 1; };` | So the editor can show and save its fields |

> **What you can ignore for now**
>
> - `BlueprintReadOnly` on the struct members. It lets Blueprints read them
>   (lesson 32).
> - `UEnum::GetValueAsString`: a ready-made function that turns an enum
>   value into text for the log.
> - The small `static` helper `MakeKindInfo` in the GameMode `.cpp`. It just
>   fills a struct in one line.

## Try it

1. Close the editor and Visual Studio.
2. **New file `OrbTypes.h`.** It goes in `Source/OrbArena/`, next to
   `Orb.h`. Make it with **File Explorer**, not Visual Studio:
   - First, in File Explorer, turn on **View > Show > File name
     extensions**. Otherwise Windows hides the end of file names, and
     "OrbTypes.h" quietly becomes `OrbTypes.h.txt`, which Unreal ignores.
   - Open `Source\OrbArena\`. Right-click an empty spot and choose **New >
     Text Document**. Rename it to exactly `OrbTypes.h` (say **Yes** when
     Windows warns about changing the extension).
   - Open it in Notepad (or any editor), paste this folder's `OrbTypes.h`,
     and save.
   - Right-click `OrbArena.uproject` and choose **Generate Visual Studio
     project files**, then open the `.sln`. `OrbTypes.h` now shows in Solution
     Explorer next to `Orb.h`.

   Why not Visual Studio's **Add > New Item**? In an Unreal project it
   usually suggests a folder under `Intermediate\`, which is a generated
   folder. The file would land where Unreal's tools never look, and you
   would get confusing "cannot open include file" errors.
3. **GameMode:** replace `PointsTable` (lesson 26) with `OrbKinds`, add
   `FindKindInfo`, and change `OrbCollected` to take an `EOrbKind` (see this
   folder's files).
4. **Orb:** replace `FName OrbType` with `EOrbKind Kind`, add `ApplyKind()`
   to set the orb's size, and pass `Kind` to `OrbCollected`.
5. Build, then open the editor.
6. Click an orb. **Kind** is now a dropdown. Make some orbs `Rare` and some
   `Golden`. In the editor they all still look the same size. Press Play:
   Golden orbs grow bigger and Rare ones shrink, while Common orbs stay as
   they were. (The sizes are set in `BeginPlay`, which runs only at Play.)
7. Open your GameMode Blueprint, click **Class Defaults** (or use the
   settings list, if it opens as a data-only Blueprint: see lesson 23, step
   7), and expand **Orb Kinds**. Each row has a kind and a struct with
   **Points** and **Scale**. Change Golden's **Points** to `50` and **Scale**
   to `2`. Compile and save the Blueprint.
8. Save (**Ctrl+Shift+S**) and Play. Golden orbs are now twice as big as
   Common ones, and worth 50.

**If an orb shows the wrong kind:** orbs you set to an `OrbType` name in
lesson 26 lost that setting, because the property no longer exists. Set
their **Kind** again.

> **A good place to stop.** This lesson is longer than most (plan on 40 to
> 50 minutes, because of the new file and the rebuilds). If you are out of
> time, stop here: everything works. "Break it" and "Try it yourself" can
> wait for next time.

## What just happened

1. UHT read `OrbTypes.h`, found `UENUM` and `USTRUCT`, and generated code
   describing them to the editor.
2. The editor used that description to draw a dropdown for `Kind` and an
   expandable table for `OrbKinds`.
3. At Play, each orb asked the GameMode for its kind's settings and scaled
   its mesh: the base size 0.5 times the kind's Scale. (Setting a scale
   *replaces* the old one, so `ApplyKind` multiplies by 0.5 itself. Read the
   comment in `Orb.cpp`.) When collected, it reported its kind, and the
   GameMode looked up the points.

All the tuning now lives in one table. Want five kinds of orb? Add an enum
value, close and rebuild once, and fill in the row.

## Break it

UHT has its own rules about which C++ it accepts. Break one.

1. Close the editor.
2. In `OrbTypes.h`, delete the `GENERATED_BODY()` line inside
   `FOrbKindInfo`.
3. Build.

The build stops before the C++ compiler runs. The first error in the Output
window names `OrbTypes.h` and the struct's line, and says something like:

```
OrbTypes.h(...): error : Expected a GENERATED_BODY() at the start of the struct
```

No `C` number, plain English, your file and line: a UHT message. It means
"you marked this struct with `USTRUCT`, so I need a place to paste my
generated code, and that place is `GENERATED_BODY()`". Put it back and build
again.

While you are here: **every** `UCLASS` and `USTRUCT` needs `GENERATED_BODY()`
as its first line, and every file with them needs its own `.generated.h` as
its last include. Those two rules cause most UHT errors you will ever see.

## Try it yourself

1. Add a fourth kind, `Cursed`, worth `-10` points. Close and rebuild (the
   enum is in a header), then add its row in the GameMode Blueprint.
2. Add a `float SpinMultiplier = 1.f;` member to `FOrbKindInfo` (with a
   `UPROPERTY`), and use it in the orb's `Tick` so Golden orbs spin faster.
   The orb will need to remember its info (or the multiplier) from
   `ApplyKind`.
3. Try `UEnum::GetValueAsString(Kind)` in the orb's `BeginPlay` to log each
   orb's kind.

## Going further

- For big tables (100 kinds of items), Unreal has **Data Tables**: a
  spreadsheet-like asset where each row is one of your `USTRUCT`s. Same idea,
  more rows.
- Try removing `: uint8` from the enum and building. Read what UHT says
  about enums used by Blueprints.

**Part 1 link:** 08 (`struct`), 09 (`enum class` and `switch`).

Next: [28-unreal-memory](../28-unreal-memory/) - hold on to an orb that
might disappear, safely.
