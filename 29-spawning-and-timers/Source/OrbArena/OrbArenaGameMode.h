// OrbArenaGameMode.h - Lesson 29: Things appear over time
//
// Template file. Keep your template's UCLASS(...) line and anything else it
// had. Changes from lesson 27 are marked NEW.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbTypes.h"
#include "OrbArenaGameMode.generated.h"   // still the last include

UCLASS()
class AOrbArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbArenaGameMode();

	void AddScore(int32 Points);
	int32 GetScore() const { return Score; }

	// An orb reports its kind when it is collected.
	void OrbCollected(EOrbKind Kind);

	// NEW (lesson 29): every orb calls this once, from its own BeginPlay, so
	// the referee knows how many are left to collect. Placed orbs and spawned
	// orbs are counted the same way.
	void RegisterOrb();

	// Settings for one kind of orb, or nullptr if the kind is not in the table.
	const FOrbKindInfo* FindKindInfo(EOrbKind Kind) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 Score = 0;

	// Settings for every kind of orb (lesson 27), tuned in the GameMode Blueprint.
	UPROPERTY(EditAnywhere, Category = "Orbs")
	TMap<EOrbKind, FOrbKindInfo> OrbKinds;

	// How many orbs are still out there. Orbs register themselves (lesson 29).
	UPROPERTY(VisibleAnywhere, Category = "Orbs")
	int32 OrbsLeft = 0;
};
