// OrbSpawner.h - Lesson 34: Capstone, Orb Arena 1.0
//
// REPLACE your OrbSpawner.h with this file. Unchanged from lesson 29 except
// the comments: the changes are all in OrbSpawner.cpp.

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

	// Which class of orb to spawn. Set it to BP_Orb (lesson 32).
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AOrb> OrbClass;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1"))
	int32 OrbsPerWave = 5;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1"))
	int32 WavesToSpawn = 3;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "0.5"))
	float SecondsBetweenWaves = 10.f;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "0"))
	float Radius = 800.f;

	int32 WavesSpawned = 0;
	FTimerHandle WaveTimer;

	void SpawnWave();
};
