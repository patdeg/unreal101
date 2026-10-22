// OrbSpawner.h - Lesson 29: Things appear over time
//
// NEW FILE. Create it with Tools > New C++ Class > Actor, named "OrbSpawner",
// then replace the wizard's code with this.
//
// Lesson 18's Spawner made monsters at runtime and owned them with unique_ptr.
// This spawner makes orbs at runtime too, but the ENGINE owns them.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orb.h"
#include "OrbSpawner.generated.h"   // always the last include

UCLASS()
class ORBARENA_API AOrbSpawner : public AActor
{
	GENERATED_BODY()

public:
	AOrbSpawner();

protected:
	virtual void BeginPlay() override;

	// WHICH class of orb to spawn, chosen in the editor.
	//
	// TSubclassOf<AOrb> means "a class: AOrb or any class that inherits from
	// it". Not an orb, a KIND of orb. Lesson 32 will set it to a Blueprint
	// child of AOrb.
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AOrb> OrbClass;

	// meta = (ClampMin = ...) stops the editor from accepting silly values.
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1"))
	int32 OrbsPerWave = 5;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1"))
	int32 WavesToSpawn = 3;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "0.5"))
	float SecondsBetweenWaves = 10.f;

	// Orbs appear inside a square this many cm from the spawner's center.
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "0"))
	float Radius = 800.f;

	// How many waves we have made so far.
	int32 WavesSpawned = 0;

	// A "ticket" for our timer. Keep it, so we can stop the timer later.
	FTimerHandle WaveTimer;

	// The function the timer calls every SecondsBetweenWaves seconds.
	void SpawnWave();
};
