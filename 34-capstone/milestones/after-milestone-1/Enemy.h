// Enemy.h - Lesson 34, AFTER MILESTONE 1 (enemy states)
//
// A snapshot: this is how Enemy.h looks at the end of milestone 1. Use it to
// compare while you work on milestone 1. Changes from lesson 30 are marked
// NEW (milestone 1).
//
// The finished version (in ../../Source/OrbArena/) is the same, except that
// milestone 3 removes bHasCaughtPlayer: from then on, the GameMode decides
// what "caught" means.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OrbTypes.h"          // NEW (milestone 1): EEnemyState
#include "Enemy.generated.h"   // always the last include

class UStaticMeshComponent;

UCLASS()
class ORBARENA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

	// NEW (milestone 1): freeze this enemy for a few seconds. Called by the
	// player's dash.
	void Stun(float Seconds);

protected:
	virtual void BeginPlay() override;

	// NEW (milestone 1): switch state, and update anything that depends on it
	// (the look). All state changes go through here, so there is one place to
	// debug.
	void SetState(EEnemyState NewState);

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0"))
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0"))
	float CatchDistance = 150.f;

	// NEW (milestone 1): how close the player must be before an idle enemy
	// notices them.
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0"))
	float SightRange = 1500.f;

	// NEW (milestone 1): the current state. VisibleAnywhere: watch it change in
	// the Details panel during Play (select the enemy in the Outliner).
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState State = EEnemyState::Idle;

	// NEW (milestone 1): seconds of stun left (only used in the Stunned state).
	float StunTimeLeft = 0.f;

	// From lesson 30: so we log "Caught!" once, not every frame.
	// (Milestone 3 removes it.)
	bool bHasCaughtPlayer = false;
};
