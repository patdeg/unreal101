# Bonus - Saving

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

Optional. Do it after the capstone's milestone 3 (or anytime after). About
25 minutes.

## Goal

Keep a high score that survives closing the game.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson adds a class:** close and rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

- **Everything in memory disappears when the game closes.** Your score lives
  in a variable; when the program ends, it is gone. To keep it, you write it
  to a file on disk and read it back next time.
- **Unreal's save games do the file work for you.** You make a small class
  that inherits from `USaveGame` and holds what you want to keep. Then:

| Step | Unreal | JavaScript (in a browser) |
|---|---|---|
| Is there a save? | `UGameplayStatics::DoesSaveGameExist(Slot, 0)` | `localStorage.getItem(k) !== null` |
| Load | `UGameplayStatics::LoadGameFromSlot(Slot, 0)` | `JSON.parse(localStorage.getItem(k))` |
| Save | `UGameplayStatics::SaveGameToSlot(Save, Slot, 0)` | `localStorage.setItem(k, JSON.stringify(obj))` |

- **`UPROPERTY()` decides what is saved.** In JavaScript, `JSON.stringify`
  writes every field. Unreal writes only the members marked `UPROPERTY()`.
  The same marker that lets the editor see a variable (lesson 21) lets the
  save system see it too.

> **What you can ignore for now**
>
> - The **user index** (the `0`). It matters for split-screen games with
>   several local players.
> - Saving many things at once (positions, inventories). Same idea, more
>   `UPROPERTY` members.

## Try it

1. In the editor, choose **Tools > New C++ Class**, click **All Classes**,
   search for **SaveGame**, pick it, and name the class **`OrbSaveGame`**.
   Then close the editor.
2. Replace `OrbSaveGame.h` with this folder's version. It is tiny:

```cpp
UCLASS()
class ORBARENA_API UOrbSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 HighScore = 0;
};
```

3. Add the five pieces from this folder's `OrbArenaGameMode.additions.txt`
   to your GameMode: a `HighScore` member, `LoadHighScore()` at the start of
   the round, `SaveHighScore()` when a round ends with a better score.
4. Build, then open the editor, save, Play. The log says
   `High score so far: 0`. Finish a round with some points.
5. **Stop playing, close the editor completely**, reopen it, and Play again:

```
LogTemp: Warning: High score so far: 27
```

The number came back from disk. Look for the file itself: in your project
folder, under `Saved/SaveGames/`, there is a file named after the slot,
`OrbArenaHighScore.sav`. Delete it to reset the high score.

## What just happened

1. At the start of the round, the GameMode checked whether the save slot
   existed and, if so, loaded it, cast it to `UOrbSaveGame` and copied the
   high score out.
2. When a round ended with a better score, it created a new save object,
   filled in `HighScore`, and asked Unreal to write it to the slot.
3. Unreal walked over the object's `UPROPERTY` members, wrote each one to
   `OrbArenaHighScore.sav`, and later read them back the same way.

## Break it

1. Close the editor. In `OrbSaveGame.h`, delete the `UPROPERTY()` line above
   `HighScore`. Build. **It builds fine.**
2. Delete `Saved/SaveGames/OrbArenaHighScore.sav` so you start clean.
3. Open the editor, Play, and finish a round with points. The log proudly says
   `New high score: ...`.
4. Close the editor completely, reopen it, and Play:

```
LogTemp: Warning: High score so far: 0
```

No error anywhere. The save file was written, but `HighScore` was not in it,
because without `UPROPERTY()` the save system cannot see the member. When it
loaded, the new save object kept its C++ default, `0`.

This is the quietest kind of bug: everything "works", and the data is just
not there. When saved data does not come back, the first question is: **is
that member a `UPROPERTY`?** Put it back, close and rebuild, and play two
rounds to check.

## Try it yourself

1. Show the high score on the HUD: a `HighScoreText` text block (lesson 33)
   and a call from `BeginPlay`.
2. Save the **best time** too: add a second `UPROPERTY` to `UOrbSaveGame`
   (for example the most seconds left when winning). Old save files do not
   have it yet; check what value it loads as.
3. Save how many rounds were played in total, and show "Round 12" on the
   HUD.

## Going further

`AsyncSaveGameToSlot` saves without freezing the game for a moment, which
matters when saves get big. Real games also store a **version number** in
the save object, so a newer game can recognize and upgrade old save files.

**Part 1 link:** 08 (a struct of data), 13 (check a pointer before you use
it). From Part 2: lesson 21 (the `UPROPERTY` marker).

Back to: [34-capstone](../34-capstone/) | Also optional:
[bonus-line-trace](../bonus-line-trace/)
