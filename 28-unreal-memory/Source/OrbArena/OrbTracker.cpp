// OrbTracker.cpp - Lesson 28: Memory the Unreal way
//
// NEW FILE. Replace the wizard's OrbTracker.cpp with this.

#include "OrbTracker.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AOrbTracker::AOrbTracker()
{
	PrimaryActorTick.bCanEverTick = true;

	// An invisible root so the tracker has a position (like lesson 20's orb).
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AOrbTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is the orb we were tracking still alive and not being destroyed?
	// If it was collected, IsValid() is now false and we look for another.
	if (!Target.IsValid())
	{
		Target = FindNearestOrb();   // a plain pointer becomes a weak pointer

		if (!Target.IsValid())
		{
			return;   // no orbs left: nothing to draw
		}
	}

	// Get() turns the weak pointer back into a normal pointer, for this frame
	// only. We just checked IsValid(), so it is safe to use right now.
	// Do not store this plain pointer anywhere; keep the weak one.
	AOrb* Orb = Target.Get();

	// A green line from the tracker to the orb, for this one frame.
	DrawDebugLine(GetWorld(), GetActorLocation(), Orb->GetActorLocation(),
		FColor::Green, false, -1.f, 0, 3.f);
}

AOrb* AOrbTracker::FindNearestOrb() const
{
	TArray<AActor*> Orbs;
	UGameplayStatics::GetAllActorsOfClass(this, AOrb::StaticClass(), Orbs);

	// Drop any orb that is already on its way out.
	//
	// RemoveAll takes a lambda (lesson 16) that answers "should this one go?".
	// IsValid(Actor) is true only if the pointer is not null AND the object is
	// not being destroyed. It is the check to use on any Unreal object pointer.
	Orbs.RemoveAll([](const AActor* Actor)
	{
		return !IsValid(Actor);
	});

	// Find the closest one, the same way you would in plain C++.
	AOrb* Nearest = nullptr;
	double BestDistance = TNumericLimits<double>::Max();   // "infinity" to start

	for (AActor* Actor : Orbs)
	{
		const double Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			Nearest = Cast<AOrb>(Actor);
		}
	}

	return Nearest;   // may be nullptr if there are no orbs: the caller checks
}
