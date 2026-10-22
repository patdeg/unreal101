// OrbSpawner.cpp - Lesson 34: Capstone, Orb Arena 1.0
//
// REPLACE your OrbSpawner.cpp with this file. Changes from lesson 29 are
// marked NEW (with the milestone number).

#include "OrbSpawner.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "OrbArenaGameMode.h"   // NEW (milestone 3)
#include "TimerManager.h"

AOrbSpawner::AOrbSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AOrbSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!OrbClass)
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s: Orb Class is empty. Select the spawner and pick an orb class in the Details panel."),
			*GetName());
		return;   // a broken spawner does not register, so it cannot block a win
	}

	// NEW (milestone 3): tell the referee "more orbs are coming".
	if (AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>())
	{
		GameMode->RegisterSpawner();
	}

	SpawnWave();
	GetWorldTimerManager().SetTimer(WaveTimer, this, &AOrbSpawner::SpawnWave,
		SecondsBetweenWaves, true);
}

void AOrbSpawner::SpawnWave()
{
	AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();

	// NEW (milestone 3): no new waves after the final whistle.
	if (GameMode != nullptr && GameMode->IsRoundOver())
	{
		GetWorldTimerManager().ClearTimer(WaveTimer);
		return;
	}

	WavesSpawned++;

	for (int32 i = 0; i < OrbsPerWave; i++)
	{
		const FVector Offset(FMath::FRandRange(-Radius, Radius),
			FMath::FRandRange(-Radius, Radius), 0.f);
		const FVector Location = GetActorLocation() + Offset;

		AOrb* Orb = GetWorld()->SpawnActor<AOrb>(OrbClass, Location, FRotator::ZeroRotator);
		if (Orb == nullptr)
		{
			continue;
		}
		Orb->SetKind(static_cast<EOrbKind>(FMath::RandRange(0, 2)));
	}

	UE_LOG(LogTemp, Warning, TEXT("Wave %d of %d: %d orbs!"), WavesSpawned, WavesToSpawn, OrbsPerWave);

	// NEW (milestone 3): that was the last wave. Stop the timer right away (not
	// one interval later) and tell the referee this spawner is done.
	if (WavesSpawned >= WavesToSpawn)
	{
		GetWorldTimerManager().ClearTimer(WaveTimer);
		if (GameMode != nullptr)
		{
			GameMode->SpawnerFinished();
		}
	}
}
