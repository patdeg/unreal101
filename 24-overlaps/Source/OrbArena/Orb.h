// Orb.h - Lesson 24: Something touched me
//
// REPLACE your Orb.h with this file (or add the lines marked NEW).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orb.generated.h"   // always the last include

class UStaticMeshComponent;
class USphereComponent;      // NEW (lesson 24): forward declaration
class UPrimitiveComponent;   // NEW (lesson 24): used in the handler below

UCLASS()
class ORBARENA_API AOrb : public AActor
{
	GENERATED_BODY()

public:
	AOrb();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Orb")
	TObjectPtr<UStaticMeshComponent> Mesh;

	// NEW (lesson 24): an invisible sphere that notices when something enters it.
	UPROPERTY(VisibleAnywhere, Category = "Orb")
	TObjectPtr<USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, Category = "Orb")
	float RotationSpeed = 90.f;

	UPROPERTY(EditAnywhere, Category = "Orb")
	FString Nickname = TEXT("Orby");

	// NEW (lesson 24) --------------------------------------------------------
	//
	// The function Unreal calls when something starts overlapping the Trigger.
	//
	// UFUNCTION() is REQUIRED here. The overlap event finds your function by
	// its NAME at runtime, and only functions marked UFUNCTION() have a name
	// Unreal can look up. (The "Break it" section shows what happens without.)
	//
	// The parameter list is long, and you must copy it exactly: it has to
	// match the event's signature. You only need one of them today:
	// OtherActor, "who touched me". Notice const FHitResult& at the end:
	// lesson 10's "look, don't copy, don't touch".
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
