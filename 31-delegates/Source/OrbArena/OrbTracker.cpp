// OrbTracker.cpp - Lesson 31: Your own events
//
// REPLACE your OrbTracker.cpp with this file. Changes from lesson 28 are
// marked NEW.

#include "OrbTracker.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"            // NEW (lesson 31): GEngine, for on-screen text
#include "Engine/World.h"             // NEW (lesson 31)
#include "Kismet/GameplayStatics.h"
#include "OrbArenaGameMode.h"         // NEW (lesson 31)

AOrbTracker::AOrbTracker()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// NEW (lesson 31) ------------------------------------------------------------
void AOrbTracker::BeginPlay()
{
	Super::BeginPlay();

	AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();
	if (GameMode == nullptr)
	{
		return;
	}

	// Subscribe: "when the GameMode broadcasts OnScoreChanged, call
	// HandleScoreChanged on this tracker."
	// In JavaScript: game.addEventListener("scoreChanged", this.handleScoreChanged)
	//
	// Same AddDynamic you used in lesson 24 for overlaps. The only difference:
	// this time the event is one YOU declared.
	GameMode->OnScoreChanged.AddDynamic(this, &AOrbTracker::HandleScoreChanged);
}

// NEW (lesson 31) ------------------------------------------------------------
void AOrbTracker::HandleScoreChanged(int32 NewScore)
{
	UE_LOG(LogTemp, Warning, TEXT("Tracker heard: score is now %d"), NewScore);

	// Show it on the game screen too, for 2 seconds.
	// GEngine can be null in some situations, so check it (lesson 13).
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,
			FString::Printf(TEXT("Score: %d"), NewScore));
	}
}

void AOrbTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target.IsValid())
	{
		Target = FindNearestOrb();
		if (!Target.IsValid())
		{
			return;
		}
	}

	AOrb* Orb = Target.Get();
	DrawDebugLine(GetWorld(), GetActorLocation(), Orb->GetActorLocation(),
		FColor::Green, false, -1.f, 0, 3.f);
}

AOrb* AOrbTracker::FindNearestOrb() const
{
	TArray<AActor*> Orbs;
	UGameplayStatics::GetAllActorsOfClass(this, AOrb::StaticClass(), Orbs);

	Orbs.RemoveAll([](const AActor* Actor)
	{
		return !IsValid(Actor);
	});

	AOrb* Nearest = nullptr;
	double BestDistance = TNumericLimits<double>::Max();
	for (AActor* Actor : Orbs)
	{
		const double Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			Nearest = Cast<AOrb>(Actor);
		}
	}
	return Nearest;
}
