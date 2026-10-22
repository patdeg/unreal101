// Orb.h - Lesson 20: Your first Actor
//
// NEW FILE. The "New C++ Class" wizard creates a file very much like this one.
// Compare yours with it, then add the comments if you like.
//
// An Actor is anything that can be placed in a level: a light, a door, a
// player, an orb. Your orb is a class that inherits from AActor, the same way
// lesson 15's Goblin inherits from Monster.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// This include MUST be the last one in the file. Unreal's code generator
// (UHT, the Unreal Header Tool) writes a file called Orb.generated.h for you,
// full of code that lets the editor see your class. It has to come after
// every other include. This lesson's "Break it" shows what happens otherwise.
#include "Orb.generated.h"

// UCLASS() is a marker for UHT. It is not normal C++: before the compiler
// runs, UHT reads your header, finds these markers, and writes extra code so
// the editor can list your class, save it in levels, and so on.
//
// ORBARENA_API is a word the wizard adds so other modules could use this class.
// It is built from your module name (OrbArena -> ORBARENA_API). Leave it.
//
// The "A" in AOrb is Unreal's naming rule: classes that are Actors start
// with A. (U for other Unreal objects, F for plain structs, E for enums.)
UCLASS()
class ORBARENA_API AOrb : public AActor
{
	// Another UHT marker. It pastes in the generated code. Always the first
	// line inside a UCLASS.
	GENERATED_BODY()

public:
	// The constructor: sets up the orb's default values (lesson 11).
	AOrb();

	// Tick is called by the engine every frame. DeltaTime is how many seconds
	// passed since the last frame (about 0.0167 at 60 frames per second).
	virtual void Tick(float DeltaTime) override;

protected:
	// Called once when the game starts (you met it in lesson 19).
	virtual void BeginPlay() override;
};
