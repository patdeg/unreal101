// OrbArenaGameMode.h - Lesson 27: Typed, data-driven orbs
//
// Template file. Keep your template's UCLASS(...) line and anything else it
// had. Changes from lesson 26 are marked NEW.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbTypes.h"                      // NEW (lesson 27): EOrbKind, FOrbKindInfo
#include "OrbArenaGameMode.generated.h"   // still the last include

UCLASS()
class AOrbArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbArenaGameMode();

	void AddScore(int32 Points);
	int32 GetScore() const { return Score; }

	// NEW (lesson 27): the orb reports its KIND (an enum), not a typed-in name.
	void OrbCollected(EOrbKind Kind);

	// NEW (lesson 27): lets an orb read the settings for its kind.
	// Returns nullptr if that kind is not in the table (lesson 26's rule).
	// The first const: the caller may look but not change the settings.
	// The last const: calling this does not change the GameMode.
	const FOrbKindInfo* FindKindInfo(EOrbKind Kind) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 Score = 0;

	// NEW (lesson 27) --------------------------------------------------------
	//
	// Lesson 26's TMap<FName, int32> becomes a map from a real type to a whole
	// struct of settings. A typo like "Goldne" is now impossible: the key can
	// only be one of the enum's values. The editor shows it as a dropdown.
	UPROPERTY(EditAnywhere, Category = "Orbs")
	TMap<EOrbKind, FOrbKindInfo> OrbKinds;

	UPROPERTY(VisibleAnywhere, Category = "Orbs")
	int32 OrbsLeft = 0;
};
