// Orb.h - Lesson 22: Actors are built from parts
//
// REPLACE your Orb.h with this file (or add the lines marked NEW).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orb.generated.h"   // always the last include

// NEW (lesson 22): a forward declaration (lesson 14).
// The header only stores a POINTER to a UStaticMeshComponent, so it only needs
// to know the name exists. The full #include goes in Orb.cpp, where we really
// use it. Unreal code does this everywhere: it keeps builds faster.
class UStaticMeshComponent;

UCLASS()
class ORBARENA_API AOrb : public AActor
{
	GENERATED_BODY()

public:
	AOrb();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// NEW (lesson 22) --------------------------------------------------------
	//
	// The orb HAS a mesh component. TObjectPtr<T> is Unreal's recommended way
	// to store a pointer to an Unreal object inside a class. Read it as
	// "UStaticMeshComponent*" (a pointer, lesson 13) with extra bookkeeping.
	//
	// VisibleAnywhere: you cannot swap the component itself in the editor,
	// but you can click it and edit ITS settings (mesh, material, size).
	UPROPERTY(VisibleAnywhere, Category = "Orb")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Orb")
	float RotationSpeed = 90.f;

	UPROPERTY(EditAnywhere, Category = "Orb")
	FString Nickname = TEXT("Orby");
};
