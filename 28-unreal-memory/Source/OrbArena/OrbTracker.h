// OrbTracker.h - Lesson 28: Memory the Unreal way
//
// NEW FILE. Create it with Tools > New C++ Class > Actor, named "OrbTracker",
// then replace the wizard's code with this.
//
// The tracker draws a green line to the nearest orb. The orb can be collected
// (destroyed) at any moment, so the tracker must never trust a pointer to it.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orb.h"
#include "OrbTracker.generated.h"   // always the last include

UCLASS()
class ORBARENA_API AOrbTracker : public AActor
{
	GENERATED_BODY()

public:
	AOrbTracker();
	virtual void Tick(float DeltaTime) override;

protected:
	// A WEAK pointer to the orb we are tracking.
	//
	// It points at the orb but does NOT keep it alive. When the orb is
	// destroyed, the weak pointer notices: Target.IsValid() becomes false.
	// Compare lesson 13: a plain pointer had no idea when its object died.
	//
	// It needs no UPROPERTY: a weak pointer never keeps anything alive, so the
	// garbage collector does not need to know about it.
	TWeakObjectPtr<AOrb> Target;

	// Searches the level for the closest orb that is still valid.
	AOrb* FindNearestOrb() const;
};
