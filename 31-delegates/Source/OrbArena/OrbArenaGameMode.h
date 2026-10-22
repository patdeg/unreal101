// OrbArenaGameMode.h - Lesson 31: Your own events
//
// Template file. Keep your template's UCLASS(...) line and anything else it
// had. Changes from lesson 29 are marked NEW.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbTypes.h"
#include "OrbArenaGameMode.generated.h"   // still the last include

// NEW (lesson 31) ------------------------------------------------------------
//
// Declare a new delegate TYPE called FOnScoreChanged.
// Read the long macro name in pieces:
//   DYNAMIC   - works with UFUNCTIONs, AddDynamic, Blueprints and the editor
//   MULTICAST - many listeners can subscribe (like addEventListener)
//   OneParam  - each broadcast sends one value: an int32 called NewScore
//
// It is lesson 16's std::function<void(int)>, with Unreal bookkeeping, and with
// room for a whole list of listeners instead of one.
// Note the commas: the type and the parameter name are separate arguments.
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

	// NEW (lesson 31): the event itself. Anyone with a pointer to the
	// GameMode can subscribe with OnScoreChanged.AddDynamic(...).
	// BlueprintAssignable: Blueprints may subscribe too.
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
};
