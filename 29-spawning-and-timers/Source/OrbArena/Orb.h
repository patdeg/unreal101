// Orb.h - Lesson 29: Things appear over time
//
// REPLACE your Orb.h with this file. Changes from lesson 27 are marked NEW.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbTypes.h"
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

	// NEW (lesson 29): lets a spawner choose the kind of a freshly spawned orb.
	// It is public because someone OUTSIDE the orb calls it.
	void SetKind(EOrbKind NewKind);

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

	// An enum (lesson 27): the Details panel shows a dropdown.
	UPROPERTY(EditAnywhere, Category = "Orb")
	EOrbKind Kind = EOrbKind::Common;

	// Read this orb's settings from the GameMode and apply them (its size).
	void ApplyKind();

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
