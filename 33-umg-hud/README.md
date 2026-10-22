# 33 - A HUD on screen

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Show the score in a corner of the screen, updated live by lesson 31's
`OnScoreChanged` event.

**Time:** about 45 minutes, longer than most lessons. There is a marked
good place to stop after Part A.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a class and changes `OrbArena.Build.cs`:** close,
> regenerate project files, and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

Unreal's UI system is called **UMG** (Unreal Motion Graphics). A screen of UI
is a **widget**. The pattern you already used for orbs works here too:

- **A C++ widget class holds the logic.** `UOrbHUDWidget : public
  UUserWidget` has a `SetScore` function and a pointer to a text block.
- **A Widget Blueprint holds the look.** `WBP_HUD` inherits from your C++
  class. In its visual designer you place the text, pick the font, size and
  color. Same split as lesson 32: C++ for rules, Blueprint for looks.
- **`BindWidget` connects the two by name.**
  `UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ScoreText;` means
  "the designer **must** contain a Text Block named exactly `ScoreText`, and
  this pointer will point at it". In JavaScript: a `getElementById` that is
  checked before the page even loads.
- **Modules.** Unreal's engine is split into **modules**, and your game
  module must list the ones it uses in `OrbArena.Build.cs`. UI lives in the
  `UMG` module. It is lesson 14's linker, one level up: "my code uses that
  library, please link it".

> **What you can ignore for now**
>
> - Anchors, canvas panels, and layout details in the widget designer. Put
>   the text in a corner and move on.
> - `FText` versus `FString` for UI text. The comment in `OrbHUDWidget.cpp`
>   says enough.
> - Creating the HUD from the PlayerController (the more usual place in
>   bigger games). The GameMode works fine for a single-player game.

## Try it

**Part A: C++.** The order matters here: the module list comes **first**.

1. **Close the editor.** Open `Source/OrbArena/OrbArena.Build.cs` (in
   Visual Studio, or in Notepad) and add `"UMG"` (and the Slate modules) as
   described in this folder's `OrbArena.Build.cs.additions.txt`. Your
   template may already list some of them: then there is less to add.
2. Right-click `OrbArena.uproject` and choose **Generate Visual Studio
   project files** (a `.Build.cs` changed). Open the `.sln`, build, and open the
   editor. Nothing looks different yet.

   Why first? In the next step the class wizard compiles your new widget
   class right away. A widget class needs the `UMG` module. Without it in
   the list, that compile fails with linker errors (`LNK2019`, "unresolved
   external symbol"), and it would look as if you broke the project.
3. In the editor, choose **Tools > New C++ Class**, click **All Classes**,
   search for **UserWidget**, select it, click **Next**, and name the class
   **`OrbHUDWidget`**. Let it finish, then close the editor.
4. Replace `OrbHUDWidget.h` and `OrbHUDWidget.cpp` with this folder's
   versions. (If they do not show in Solution Explorer, regenerate the
   project files as in step 2 and reopen the `.sln`.)
5. In the GameMode, add `HUDWidgetClass` and `HUDWidget` to the header, and
   the widget code to `BeginPlay` (this folder's `OrbArenaGameMode.h/.cpp`).
   The core:

```cpp
UOrbHUDWidget* Widget = CreateWidget<UOrbHUDWidget>(GetWorld(), HUDWidgetClass);
Widget->AddToViewport();
Widget->SetScore(Score);
OnScoreChanged.AddDynamic(Widget, &UOrbHUDWidget::SetScore);
```

6. Build, then open the editor. Play once: the game works as before, and
   the log shows a yellow warning that starts with `HUD Widget Class is
   empty: no HUD`. That is expected: the widget's look does not exist yet.

> **A good place to stop.** This lesson is long (plan on about 45 minutes,
> with all the rebuilds). Part A is all the C++, and the game still runs
> without a HUD. If you are out of time, stop here and do Parts B and C
> next time. They are editor-only: no more rebuilds.

**Part B: the Widget Blueprint.**

7. In the Content Browser, right-click an empty spot and choose **User
   Interface > Widget Blueprint**. When it asks for a parent class, pick
   **OrbHUDWidget** (use **All Classes** and search if it is not listed). Name
   it **`WBP_HUD`**.
8. Open it. In the **Designer**, from the **Palette** on the left, drag a
   **Canvas Panel** onto the empty hierarchy (if there is not one already),
   then drag a **Text** widget onto the canvas, near the **top right**
   corner. (Not the top left: the tracker's green on-screen messages from
   lesson 31 print there, and the two would overlap.)
9. With the text selected, rename it at the top of the **Details** panel to
   exactly **`ScoreText`**. If there is an **Is Variable** tick box, make
   sure it is ticked. Make the font bigger if you like.
10. **Compile** and **Save**.

**Part C: tell the GameMode which widget to use.**

11. Open your GameMode Blueprint and click **Class Defaults** (or use the
    settings list, if it opens as a data-only Blueprint: see lesson 23,
    step 7). In the **HUD** section, set **HUD Widget Class** to
    **WBP_HUD**. Compile and save.

    Careful: the GameMode also has a field called **HUD Class**, which comes
    from Unreal's `AGameModeBase` and belongs to an older HUD system. Leave
    that one alone. (It is also why our property is not called `HUDClass`:
    UHT refuses a `UPROPERTY` with the same name as one in a parent class.)
12. Save (**Ctrl+Shift+S**) and Play. "Score: 0" appears in the top right
    corner. Collect an orb: it updates instantly. (The tracker's green text
    still flashes at the top left. That is its debug message from lesson 31;
    your HUD is the one at the top right.)

## What just happened

1. The GameMode's `BeginPlay` created one object of `WBP_HUD` (whose parent
   is your C++ class) and added it to the screen.
2. When it was created, `BindWidget` found the Text Block named `ScoreText`
   in the design and filled your C++ pointer with it.
3. The GameMode subscribed the widget's `SetScore` to `OnScoreChanged`. From
   then on, every `AddScore` broadcast reached the HUD, and the HUD updated
   its text. The GameMode never calls the HUD directly.

That is lesson 31's promise kept: a new listener, and `AddScore` did not
change.

## Break it

`BindWidget` is a contract between your C++ and the designer, and the
editor checks it.

1. Open `WBP_HUD`, select the text, and rename it to **`Score`** (dropping
   `Text`).
2. Click **Compile**.

The Blueprint refuses to compile. The **Compiler Results** panel at the
bottom shows an error that says something like:

```
A required widget binding "ScoreText" of type Text Block was not found.
```

Read it like a contract: your C++ class **requires** a widget named
`ScoreText` of type `Text Block`, and the design does not have one. Notice
where the error appears: not in Visual Studio, not at runtime, but in the
Widget Blueprint editor. Unreal errors come from many places (the C++
compiler, UHT, the linker, the Blueprint compiler, the running game), and
part of the skill is knowing which window to look in.

Rename it back to `ScoreText`, then compile and save.

## Try it yourself

1. Change the font size and color in the designer. No C++ needed.
2. Add an "Orbs left" text: a second `BindWidget` property `OrbsLeftText`,
   a `SetOrbsLeft(int32)` function, and a matching delegate on the GameMode
   (you may have built one in lesson 31's "Try it yourself").
3. Make the score text flash: in `WBP_HUD`, create an **Animation** that
   scales the text up and down, and play it from the Blueprint graph. (Hint:
   `SetScore` could call a `BlueprintImplementableEvent` such as
   `OnScoreUpdated`, lesson 32.)

## Going further

Instead of `UPROPERTY(meta = (BindWidget))`, there is
`UPROPERTY(meta = (BindWidgetOptional))`, for a widget the designer **may**
leave out. Then the pointer can be `nullptr`, and your `if (ScoreText ==
nullptr)` check really matters.

**Part 1 link:** 11 (a class with a clear job), 14 (linking: the
`.Build.cs` module list), 16 (callbacks).

Next: [34-capstone](../34-capstone/) - finish Orb Arena 1.0.
