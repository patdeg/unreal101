// OrbTypes.h - Lesson 34: Capstone, Orb Arena 1.0
//
// REPLACE your OrbTypes.h with this file. New in the capstone: EEnemyState.

#pragma once

#include "CoreMinimal.h"
#include "OrbTypes.generated.h"   // always the last include

UENUM(BlueprintType)
enum class EOrbKind : uint8
{
	Common,
	Rare,
	Golden
};

USTRUCT(BlueprintType)
struct FOrbKindInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb")
	int32 Points = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb")
	float Scale = 1.f;
};

// NEW (capstone, milestone 1) ------------------------------------------------
//
// What an enemy is doing right now. Remember lesson 09's monster Stance
// (Normal, Defending, Stunned) in the Text Arena? Same idea, in 3D.
// Exactly one state at a time, and a switch in Tick decides what each one does.
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,      // standing still, waiting to see the player
	Chase,     // running at the player
	Stunned    // hit by a dash: frozen for a moment
};
