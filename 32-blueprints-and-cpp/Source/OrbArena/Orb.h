// Orb.h - Lesson 32: C++ and Blueprints together
//
// REPLACE your Orb.h with this file. Changes from lesson 29 are marked NEW.

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

	// Lets a spawner choose the kind of a freshly spawned orb (lesson 29).
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
	// NEW (lesson 32): BlueprintReadOnly lets the orb's Blueprint READ the
	// kind (for example, to pick a color or a sound per kind).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb")
	EOrbKind Kind = EOrbKind::Common;

	// Read this orb's settings from the GameMode and apply them (its size).
	void ApplyKind();

	// NEW (lesson 32) --------------------------------------------------------
	//
	// A hook for Blueprints. C++ DECLARES it and CALLS it; a Blueprint child
	// class draws what it does, as an event node in its graph.
	//
	// It is lesson 15's virtual function, with the override drawn in a graph
	// instead of typed in a child class.
	//
	// IMPORTANT: you do NOT write a body for this function in Orb.cpp.
	// UHT writes one for you (in Orb.gen.cpp) that forwards the call to the
	// Blueprint. If no Blueprint implements it, calling it simply does nothing.
	UFUNCTION(BlueprintImplementableEvent, Category = "Orb")
	void OnCollected();

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
