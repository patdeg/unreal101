// OrbTracker.h - Lesson 31: Your own events
//
// REPLACE your OrbTracker.h with this file. Changes from lesson 28 are marked NEW.

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
	// NEW (lesson 31): subscribe to the GameMode's event when the game starts.
	virtual void BeginPlay() override;

	// NEW (lesson 31): the listener. The GameMode calls it on every Broadcast.
	//
	// Two rules for a dynamic delegate listener:
	//   1. It must be a UFUNCTION() (same reason as lesson 24: it is found by
	//      name).
	//   2. Its parameters must match the delegate's EXACTLY: one int32.
	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);

	TWeakObjectPtr<AOrb> Target;

	AOrb* FindNearestOrb() const;
};
