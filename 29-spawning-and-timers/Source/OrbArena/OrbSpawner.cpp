// OrbSpawner.cpp - Lesson 29: Things appear over time
//
// NEW FILE. Replace the wizard's OrbSpawner.cpp with this.

#include "OrbSpawner.h"

#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AOrbSpawner::AOrbSpawner()
{
	// The spawner works with a timer, not every frame, so it does not tick.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AOrbSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Things set in the editor can be missing. Check, and say clearly what to
	// do about it. (This is the "Break it" of this lesson.)
	if (!OrbClass)
	{
		UE_LOG(LogTemp, Error,
			TEXT("%s: Orb Class is empty. Select the spawner and pick an orb class in the Details panel."),
			*GetName());
		return;
	}

	// The first wave right away...
	SpawnWave();

	// ...then one every SecondsBetweenWaves seconds.
	//
	// SetTimer(ticket, object, function, seconds, repeat?)
	// In JavaScript: setInterval(() => this.spawnWave(), 10000)
	// Like lesson 23's BindAction, you pass the object and the ADDRESS of a
	// member function, and the engine calls it later.
	GetWorldTimerManager().SetTimer(WaveTimer, this, &AOrbSpawner::SpawnWave,
		SecondsBetweenWaves, true);
}

void AOrbSpawner::SpawnWave()
{
	// Done? Stop the timer (like clearInterval) and stop here.
	if (WavesSpawned >= WavesToSpawn)
	{
		GetWorldTimerManager().ClearTimer(WaveTimer);
		return;
	}
	WavesSpawned++;

	for (int32 i = 0; i < OrbsPerWave; i++)
	{
		// A random spot around the spawner. FVector + FVector adds the X, Y and
		// Z parts separately. Z stays the spawner's height.
		const FVector Offset(FMath::FRandRange(-Radius, Radius),
			FMath::FRandRange(-Radius, Radius), 0.f);
		const FVector Location = GetActorLocation() + Offset;

		// Create the orb. The engine builds it, runs its constructor and its
		// BeginPlay (where it registers with the GameMode), puts it in the
		// level, and from now on OWNS it. We get back a plain pointer to look
		// at it (or nullptr if spawning failed).
		AOrb* Orb = GetWorld()->SpawnActor<AOrb>(OrbClass, Location, FRotator::ZeroRotator);
		if (Orb == nullptr)
		{
			continue;   // skip this one, try the next
		}

		// A random kind: 0, 1 or 2, turned into the enum (lesson 27).
		Orb->SetKind(static_cast<EOrbKind>(FMath::RandRange(0, 2)));
	}

	UE_LOG(LogTemp, Warning, TEXT("Wave %d of %d: %d orbs!"), WavesSpawned, WavesToSpawn, OrbsPerWave);
}
