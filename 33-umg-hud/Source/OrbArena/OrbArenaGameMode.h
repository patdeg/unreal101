// OrbArenaGameMode.h - Lesson 33: A HUD on screen
//
// Template file. Keep your template's UCLASS(...) line and anything else it
// had. Changes from lesson 31 are marked NEW.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbTypes.h"
#include "OrbArenaGameMode.generated.h"   // still the last include

class UOrbHUDWidget;   // NEW (lesson 33): forward declaration

// "The score changed" event (lesson 31).
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

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

	// Every orb calls this once, from its own BeginPlay (lesson 29).
	void RegisterOrb();

	// Settings for one kind of orb, or nullptr if the kind is not in the table.
	const FOrbKindInfo* FindKindInfo(EOrbKind Kind) const;

	// Anyone can subscribe with OnScoreChanged.AddDynamic(...) (lesson 31).
	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;

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

	// NEW (lesson 33) --------------------------------------------------------
	//
	// WHICH widget class to show: set it to WBP_HUD in the GameMode
	// Blueprint's Class Defaults. TSubclassOf, like the spawner's OrbClass
	// (lesson 29): a class, not an object.
	//
	// Why not call it "HUDClass"? AGameModeBase ALREADY has a property with
	// that name (for an older HUD system), and UHT refuses a UPROPERTY that
	// hides one in a parent class. Pick specific names in classes with big
	// parents.
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UOrbHUDWidget> HUDWidgetClass;

	// The HUD object we created. UPROPERTY() so the garbage collector knows
	// we are holding it (lesson 28): a plain member pointer would be a trap.
	UPROPERTY()
	TObjectPtr<UOrbHUDWidget> HUDWidget;
};
