// Orb.h - Lesson 26: Unreal's containers
//
// REPLACE your Orb.h with this file (or add the lines marked NEW).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	// NEW (lesson 26): what type of orb this is. The GameMode's PointsTable
	// says how many points each type is worth. Set it per orb in the editor.
	UPROPERTY(EditAnywhere, Category = "Orb")
	FName OrbType = FName(TEXT("Common"));

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
