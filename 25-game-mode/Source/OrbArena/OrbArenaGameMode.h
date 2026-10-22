// OrbArenaGameMode.h - Lesson 25: The referee
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

	// NEW (lesson 25) --------------------------------------------------------
	//
	// Anyone can ASK the referee to add points. Nobody can set the score
	// directly: Score is protected below. Same idea as lesson 11's Fighter,
	// where only the class could change its own HP.
	void AddScore(int32 Points);

	// A small "getter". const means: calling this never changes the GameMode.
	int32 GetScore() const { return Score; }

protected:
	virtual void BeginPlay() override;

	// NEW (lesson 25)
	// VisibleAnywhere: you can WATCH the score in the Details panel while the
	// game runs, but not edit it there.
	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 Score = 0;
};
