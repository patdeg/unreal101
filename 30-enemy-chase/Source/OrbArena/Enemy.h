// Enemy.h - Lesson 30: Something chases you
//
// NEW FILE. Create it with Tools > New C++ Class. In the wizard, pick
// "Character" as the parent class (not Actor), and name it "Enemy". Then
// replace the wizard's code with this.
//
// Why a Character and not an Actor? ACharacter already knows how to walk:
// it has a capsule for collision, gravity, and a movement component that
// handles slopes and stairs. Like the player, just without a human driving it.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"   // always the last include

class UStaticMeshComponent;

UCLASS()
class ORBARENA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// A simple box body so you can see the enemy. (A Character comes with a
	// skeletal mesh slot for animated models; we leave that empty.)
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	TObjectPtr<UStaticMeshComponent> Body;

	// Walking speed in cm per second. The player walks at roughly 500.
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0"))
	float WalkSpeed = 300.f;

	// How close (in cm) counts as "caught".
	UPROPERTY(EditAnywhere, Category = "Enemy", meta = (ClampMin = "0"))
	float CatchDistance = 150.f;

	// So we log "Caught!" once, not every frame.
	bool bHasCaughtPlayer = false;
};
