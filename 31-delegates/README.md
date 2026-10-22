# 31 - Your own events

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

## Goal

Make the GameMode announce "the score changed" to anyone who wants to know,
and have the tracker listen.

> **Rebuild rule.** Changed only a function body in a `.cpp`? Live Coding
> (Ctrl+Alt+F11). Changed a `.h`, added a `UPROPERTY`/`UFUNCTION`, or added a
> class? Close the editor, build in Visual Studio, reopen.
> **This lesson declares a delegate and adds a `UFUNCTION`:** close and
> rebuild.
>
> **Save before you press Play** (Ctrl+Shift+S).

## The new idea

Right now, if three things care about the score (a tracker, a HUD, a sound),
the GameMode would have to know all three and call each one. Every new
listener means editing the GameMode. That does not scale.

Events fix this. In JavaScript you already know them:

```js
game.addEventListener("scoreChanged", (e) => updateHud(e.detail));  // listener
game.dispatchEvent(new CustomEvent("scoreChanged", { detail: 42 })); // announcer
```

Unreal calls them **delegates**. Three steps:

| Step | Unreal | JavaScript |
|---|---|---|
| 1. Declare the event's shape | `DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);` | (deciding what `detail` holds) |
| 2. Announce | `OnScoreChanged.Broadcast(Score);` | `dispatchEvent(...)` |
| 3. Listen | `GameMode->OnScoreChanged.AddDynamic(this, &AOrbTracker::HandleScoreChanged);` | `addEventListener(...)` |

- **It is lesson 16's `std::function`, grown up.** A `std::function` (such
  as `std::function<void(const Monster&)> onDeath` from lesson 16's "Try it
  yourself") holds one function. A **multicast** delegate holds a whole list
  of listeners and calls them all.
- **You already used one.** Lesson 24's `OnComponentBeginOverlap` is a
  delegate that Unreal declared. Today you declare your own.
- **The announcer does not know the listeners.** The GameMode just calls
  `Broadcast`. Who listens is their business. Lesson 33's HUD will listen to
  this same event without the GameMode changing at all.

> **What you can ignore for now**
>
> - The other delegate flavors (`DECLARE_DELEGATE`,
>   `DECLARE_MULTICAST_DELEGATE` without DYNAMIC). The dynamic multicast one
>   is the one that works with Blueprints and the editor.
> - `BlueprintAssignable`: lets Blueprints subscribe too.
> - `FString::Printf`: builds an `FString` the way `UE_LOG` builds a line.

## Try it

1. Close the editor.
2. **GameMode header:** above `UCLASS()`, declare the delegate type; inside
   the class, in `public:`, add the event member (this folder's
   `OrbArenaGameMode.h`):

```cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
// ...
UPROPERTY(BlueprintAssignable, Category = "Score")
FOnScoreChanged OnScoreChanged;
```

3. **GameMode `.cpp`:** at the end of `AddScore`, add
   `OnScoreChanged.Broadcast(Score);`.
4. **Tracker:** add `BeginPlay` (to subscribe) and the `UFUNCTION`
   `HandleScoreChanged` (to listen). See this folder's `OrbTracker.h` and
   `OrbTracker.cpp`.
5. Build, then open the editor. Make sure an **OrbTracker** is still in the
   level (from lesson 28).
6. Save, Play, and collect orbs. Green text appears at the top left of the game
   screen, and the log shows both sides:

```
LogTemp: Warning: Score: 5
LogTemp: Warning: Tracker heard: score is now 5
```

Drag a **second** tracker into the level and play again. Both hear every
change: a multicast delegate has a list of listeners.

## What just happened

1. The macro created a new type, `FOnScoreChanged`: "an event that sends one
   `int32`".
2. The GameMode got a member of that type, `OnScoreChanged`: an empty list
   of listeners.
3. At `BeginPlay`, each tracker added itself to the list with `AddDynamic`.
4. Every `AddScore` called `Broadcast(Score)`, which went down the list and
   called `HandleScoreChanged(Score)` on every tracker still alive.

The GameMode never mentions `AOrbTracker`. You could delete the tracker
class and the GameMode would still build.

## Break it

A delegate is a contract about **types**. Break the contract.

1. Close the editor.
2. In `OrbTracker.h` **and** `OrbTracker.cpp`, change the listener to take a
   `float` instead of an `int32`:
   `void HandleScoreChanged(float NewScore);`
3. Build.

The build fails at the `AddDynamic` line in `OrbTracker.cpp`. The error is
long, because `AddDynamic` is a macro that expands into template code, and
template errors are always long. It looks something like:

```
OrbTracker.cpp(...): error C2664: '... __Internal_AddDynamic<AOrbTracker>(UserClass *, void (__cdecl AOrbTracker::* )(int32), FName)': cannot convert argument 2 from 'void (__cdecl AOrbTracker::* )(float)' to 'void (__cdecl AOrbTracker::* )(int32)'
```

Do not panic at the length. Use the rules you already have:

1. Read the **first** error only.
2. Find the line that names **your** file (`OrbTracker.cpp` and its line).
3. Look for the words "cannot convert" and compare the two types it lists:
   `(float)` versus `(int32)`. That is the whole story: the function you
   gave takes a `float`, the event sends an `int32`.

Change it back to `int32` in both files and build again.

## Try it yourself

1. Add a second delegate, `FOnOrbsLeftChanged`, that sends `OrbsLeft`, and
   broadcast it from `RegisterOrb` and `OrbCollected`. Make the tracker log
   it too.
2. Make the tracker unsubscribe when it is destroyed: override `EndPlay` and
   call `GameMode->OnScoreChanged.RemoveDynamic(this,
   &AOrbTracker::HandleScoreChanged);`. (Unreal also skips listeners that
   no longer exist, but being tidy is a good habit.)
3. Change the on-screen message color to `FColor::Yellow` for scores above
   50.

## Going further

Unreal also has non-dynamic delegates, which are faster and can hold a
lambda directly (for example with `AddLambda`). They do not work with
Blueprints or `UFUNCTION`s, and a lambda that captures `this` can outlive
the object it points at: lesson 28's dangling pointer, in a new disguise.
Look up `AddWeakLambda` and `AddUObject` to see how Unreal protects you.

**Part 1 link:** 11 (member functions), 16 (lambdas and `std::function`).

Next: [32-blueprints-and-cpp](../32-blueprints-and-cpp/) - let a Blueprint
add the sparkle while C++ keeps the rules.
