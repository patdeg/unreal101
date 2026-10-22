// OrbArenaGameMode.h - Lesson 19: Unreal says hello
//
// This file came with the Third Person C++ template. DO NOT replace your copy
// with this one: your template version may contain extra lines (for example a
// different UCLASS(...) line or an ORBARENA_API word) and those lines matter.
// ADD only the lines marked NEW.
//
// What you can ignore for now:
//   - CoreMinimal.h: "the basics of Unreal", like <iostream> in Part 1.
//   - The .generated.h include and GENERATED_BODY(): lesson 20 explains them.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbArenaGameMode.generated.h"

// The GameMode is the "referee" of a level. Unreal creates one for you when
// the game starts. It is a class that inherits from AGameModeBase, just like
// lesson 15's Goblin inherits from Monster.
//
// Your template's line may say UCLASS(minimalapi) or UCLASS(abstract) or
// mention ORBARENA_API. Keep whatever yours says.
UCLASS()
class AOrbArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbArenaGameMode();

protected:
	// NEW (lesson 19)
	// BeginPlay is a virtual function on every Actor. The engine calls it once,
	// when the game starts. "override" asks the compiler to check that a
	// virtual function with this exact name and signature really exists in a
	// parent class (lesson 15's free insurance).
	virtual void BeginPlay() override;
};
