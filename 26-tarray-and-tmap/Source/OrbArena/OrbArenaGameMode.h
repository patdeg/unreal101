// OrbArenaGameMode.h - Lesson 26: Unreal's containers
//
// Template file. Keep your template's UCLASS(...) line and anything else it
// had; ADD the lines marked NEW.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbArenaGameMode.generated.h"

UCLASS()
class AOrbArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbArenaGameMode();

	void AddScore(int32 Points);
	int32 GetScore() const { return Score; }

	// NEW (lesson 26): an orb calls this when it is collected, and says what
	// type it was. The referee decides how many points that is worth.
	void OrbCollected(FName OrbType);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 Score = 0;

	// NEW (lesson 26) --------------------------------------------------------
	//
	// TMap<Key, Value> is Unreal's std::map (lesson 07): look up a value by a
	// key. Here: orb type name -> points.
	//
	// FName is Unreal's "name tag" text type: very fast to compare, made for
	// identifiers like "Common" or "Golden". (FString is for text you change.)
	//
	// EditAnywhere: you can change the table in the GameMode Blueprint's Class
	// Defaults, without recompiling.
	UPROPERTY(EditAnywhere, Category = "Orbs")
	TMap<FName, int32> PointsTable;

	// How many orbs are still out there. Counted at BeginPlay.
	UPROPERTY(VisibleAnywhere, Category = "Orbs")
	int32 OrbsLeft = 0;
};
