# 23 - The player presses a key

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Make the player dash forward when you press **Left Shift**.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson changes the character's header:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

This lesson has the most clicking in the whole course. Go slowly. Half of
it is C++, half is wiring things up in the editor, and both halves matter.

## The new idea

Unreal's input system is called **Enhanced Input**. It has three pieces:

| Piece | What it is | JavaScript comparison |
|---|---|---|
| **Input Action** (`IA_Dash`) | A named thing the player can do: "Dash". It does not know about keys. | The event name, `"dash"` |
| **Input Mapping Context** (`IMC_Default`) | A table: "Left Shift means Dash". | Which key fires the event |
| **`BindAction` in C++** | "When Dash starts, call `Dash()` on this character." | `addEventListener("dash", this.dash.bind(this))` |

Splitting it like this means you can change keys, or add a gamepad button,
without touching C++.

- **You will read code you did not write.** The template's character
  already binds jump, move and look. You add your binding next to theirs, in
  the same style. Reading someone else's code and fitting in is a real
  skill.
- **`ETriggerEvent::Started`** is an `enum class` value (lesson 09): "the
  moment the key goes down". Others include `Triggered` (every frame while
  held) and `Completed` (released).
- **`&AOrbArenaCharacter::Dash`** is the address of a member function. You
  pass the function itself, without calling it, like passing a function
  value in lesson 16.

> **What you can ignore for now**
>
> - Most of `OrbArenaCharacter.h` and `.cpp`: the camera boom, the movement
>   functions, the other input actions.
> - Value types of input actions (Digital, Axis1D, Axis2D). A dash is a
>   simple button, which is the default.
> - Triggers and modifiers on mappings.

## Try it

**Part A: the editor (keys).**

1. In the Content Browser, find the template's input folder. It is the
   folder that contains assets whose names start with `IA_` (such as
   `IA_Jump`) and `IMC_` (such as `IMC_Default`). Search for `IA_Jump` if
   you cannot find it.
2. In that folder, right-click an empty spot and choose **Input > Input
   Action**. Name it **`IA_Dash`**. You do not need to change anything inside
   it.
3. Open the mapping context the template uses (for example `IMC_Default`).
   Click **+** next to **Mappings**, choose **IA_Dash** in the new row's
   dropdown, then click the keyboard icon next to it and press **Left
   Shift**. Save.

> **If your template looks different.** Epic moves this setup around
> between engine versions. The input folder may sit somewhere else, there
> may be more than one `IMC_` asset (if so, use the one that already maps
> `IA_Jump`), and in recent templates the mapping context is switched on by
> the **player controller** instead of the character. None of that changes
> what you do here: you edit the mapping context asset itself, and the C++
> below still goes in the character, next to the template's own
> `BindAction` lines.

**Part B: the C++.**

4. Close the editor.
5. Open `OrbArenaCharacter.h` and `OrbArenaCharacter.cpp`. **Do not replace
   them.** Add the three pieces from this folder's
   `Source/OrbArena/OrbArenaCharacter.additions.txt`:
   - in the header: a `DashAction` property, a `DashStrength` property, and
     a `Dash()` declaration;
   - in `SetupPlayerInputComponent`: one `BindAction` line, next to the
     template's own;
   - at the bottom of the `.cpp`: the `Dash()` function.

The binding is the key line:

```cpp
EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started,
	this, &AOrbArenaCharacter::Dash);
```

6. Build, then open the editor.

**Part C: connect the asset to the code.**

7. Find the character Blueprint the level uses (search for
   `BP_ThirdPersonCharacter`). Open it. Click **Class Defaults** in the
   toolbar. In the **Details** panel, in the **Input** section, set **Dash
   Action** to **IA_Dash**. Click **Compile**, then **Save**.

   **No Class Defaults button?** Some Blueprints hold only settings and no
   graphs (Unreal calls them "data-only" Blueprints). They open in a simpler
   window: a plain list of settings, with a note about a data-only Blueprint
   and a link **Open Full Blueprint Editor**. That list *is* the Class
   Defaults, so set **Dash Action** right there. This happens again in later
   lessons, with the GameMode Blueprint.

Why here? `DashAction` is a `UPROPERTY` on your C++ class. The Blueprint is
a child of your C++ class, so it has the property too, and that is where the
template sets all its input assets.

8. Save (**Ctrl+Shift+S**) and Play. Run forward and press **Left Shift**. You
   shoot forward, and the log says `LogTemp: Warning: Dash!`.

## What just happened

1. Pressing Left Shift was looked up in the mapping context: "Left Shift
   means IA_Dash".
2. Enhanced Input fired the `Started` event for `IA_Dash`.
3. Your `BindAction` had said "on `IA_Dash` `Started`, call `Dash` on this
   character", so the engine called your function.
4. `Dash` used `LaunchCharacter` to replace the character's horizontal
   speed with a big push in the facing direction.

## Break it

This one has **no compiler error at all**, which is exactly the point.

1. Open `BP_ThirdPersonCharacter`, click **Class Defaults**, and clear **Dash
   Action** (set it to **None**). Compile and save.
2. Play and press **Left Shift**. Nothing happens.
3. Now look at the Output Log. Near the start of Play there is a red line:

```
LogTemp: Error: DashAction is empty. Open the character Blueprint and set Dash Action to IA_Dash.
```

That message exists only because the code checks `if (DashAction)` and says
what is wrong. Without that check, you would get silence. Set **Dash
Action** back to **IA_Dash**.

Now break the other half: open `IMC_Default` and delete the `IA_Dash` mapping
(or change the key to something odd). Play and press **Left Shift**. Nothing
happens, and this time the log says nothing either. To find out where the
chain breaks, put a breakpoint on the `UE_LOG` line in `Dash()` (lesson 12's
skill: start the editor with **F5** from Visual Studio). It is never hit, so
the function is never called, so the problem is before C++: the wiring.

Lesson: **half of Unreal bugs are editor wiring, not code**. When nothing
happens and nothing errors, check each link in the chain, and write code
that complains clearly when something is missing. Put the mapping back.

## Try it yourself

1. Change **Dash Strength** (in the **Dash** section of the Blueprint's Class
   Defaults; no rebuild needed) until the dash feels right. On the ground,
   friction brakes you quickly, so if 1500 feels short, try 2500, or try
   number 3 below.
2. Add a second key for Dash in the mapping context, such as a gamepad
   button. No C++ change.
3. Make the dash go a little upward too: add `FVector(0.f, 0.f, 300.f)` to
   the launch velocity and pass `true` for the third argument. This only
   changes a function body, so Live Coding works.

## Going further

Add a cooldown so the player cannot dash every frame. Store the time of the
last dash in a `float LastDashTime = -100.f;` member. In `Dash()`, read the
current time with `GetWorld()->GetTimeSeconds()` (add `#include
"Engine/World.h"`), and `return` early if less than 1.5 seconds have passed.

**Part 1 link:** 09 (`enum class`), 11 (member functions), 16 (passing a
function as a value).

Next: [24-overlaps](../24-overlaps/) - collect an orb by touching it.
