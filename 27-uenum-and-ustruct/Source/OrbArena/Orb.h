// Orb.h - Lesson 27: Typed, data-driven orbs
//
// REPLACE your Orb.h with this file. Changes from lesson 26 are marked NEW.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbTypes.h"        // NEW (lesson 27): EOrbKind
#include "Orb.generated.h"   // always the last include

class UStaticMeshComponent;
class USphereComponent;
class UPrimitiveComponent;

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

	UPROPERTY(VisibleAnywhere, Category = "Orb")
	TObjectPtr<USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, Category = "Orb")
	float RotationSpeed = 90.f;

	UPROPERTY(EditAnywhere, Category = "Orb")
	FString Nickname = TEXT("Orby");

	// NEW (lesson 27): replaces lesson 26's "FName OrbType".
	// An enum: the Details panel shows a dropdown with Common, Rare, Golden.
	// No more typos.
	UPROPERTY(EditAnywhere, Category = "Orb")
	EOrbKind Kind = EOrbKind::Common;

	// NEW (lesson 27): read this orb's settings from the GameMode and apply
	// them (for now: its size).
	void ApplyKind();

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
