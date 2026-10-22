// OrbArenaGameMode.h - Lesson 34, AFTER MILESTONE 3 (round timer, win and lose)
//
// A snapshot: how OrbArenaGameMode.h looks at the end of milestone 3 (it
// does not change in milestone 4). Template file: keep your template's
// UCLASS(...) line and anything else it had. Changes from lesson 33 are
// marked NEW (milestone 3).
//
// Milestone 5 adds RestartRound: see the finished version in
// ../../Source/OrbArena/OrbArenaGameMode.h.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbTypes.h"
#include "OrbArenaGameMode.generated.h"   // still the last include

class UOrbHUDWidget;

// The referee's announcements (lesson 31).
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
// NEW (milestone 3): the clock ticked; the round ended.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, int32, SecondsLeft);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, bWon);

UCLASS()
class AOrbArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOrbArenaGameMode();

	void AddScore(int32 Points);
	int32 GetScore() const { return Score; }

	void OrbCollected(EOrbKind Kind);
	void RegisterOrb();
	const FOrbKindInfo* FindKindInfo(EOrbKind Kind) const;

	// NEW (milestone 3) ------------------------------------------------------
	// Spawners register themselves, like orbs do, and say when they are done.
	// The round is won when every spawner is done AND every orb is collected.
	void RegisterSpawner();
	void SpawnerFinished();

	// An enemy calls this when it reaches the player.
	void PlayerCaught();

	bool IsRoundOver() const { return bRoundOver; }

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;

	// NEW (milestone 3)
	UPROPERTY(BlueprintAssignable, Category = "Round")
	FOnTimeChanged OnTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Round")
	FOnGameOver OnGameOver;

protected:
	virtual void BeginPlay() override;

	// NEW (milestone 3): called by a timer once per second.
	void TickRound();

	// NEW (milestone 3): checks "all spawners done and no orbs left".
	void CheckForWin();

	// NEW (milestone 3): the one place where a round ends, win or lose.
	void EndRound(bool bWon);

	UPROPERTY(VisibleAnywhere, Category = "Score")
	int32 Score = 0;

	UPROPERTY(EditAnywhere, Category = "Orbs")
	TMap<EOrbKind, FOrbKindInfo> OrbKinds;

	UPROPERTY(VisibleAnywhere, Category = "Orbs")
	int32 OrbsLeft = 0;

	// NEW (milestone 3) ------------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Round", meta = (ClampMin = "10"))
	int32 RoundSeconds = 90;

	UPROPERTY(VisibleAnywhere, Category = "Round")
	int32 TimeLeft = 0;

	UPROPERTY(VisibleAnywhere, Category = "Round")
	bool bRoundOver = false;

	int32 SpawnersRunning = 0;
	FTimerHandle RoundTimer;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UOrbHUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UOrbHUDWidget> HUDWidget;
};
