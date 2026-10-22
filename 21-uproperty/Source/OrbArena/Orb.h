// Orb.h - Lesson 21: Knobs in the editor
//
// REPLACE your Orb.h with this file (or add the lines marked NEW).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orb.generated.h"   // always the last include

UCLASS()
class ORBARENA_API AOrb : public AActor
{
	GENERATED_BODY()

public:
	AOrb();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// NEW (lesson 21) --------------------------------------------------------
	//
	// UPROPERTY() is another marker for UHT (like UCLASS). It tells Unreal
	// "this member variable is special: let the editor see it".
	//
	//   EditAnywhere      - you can change it in the Details panel, both on the
	//                       class defaults and on each placed orb.
	//   Category = "Orb"  - groups it under an "Orb" heading in the panel.
	//
	// The "= 90.f" is a normal C++ default value (lesson 08). It is what a NEW
	// orb starts with. An orb you already placed and edited keeps its own value.
	UPROPERTY(EditAnywhere, Category = "Orb")
	float RotationSpeed = 90.f;

	// FString is Unreal's std::string (lesson 06). TEXT("...") makes the
	// literal Unreal-friendly.
	UPROPERTY(EditAnywhere, Category = "Orb")
	FString Nickname = TEXT("Orby");
};
