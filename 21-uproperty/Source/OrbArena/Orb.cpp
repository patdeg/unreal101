// Orb.cpp - Lesson 21: Knobs in the editor
//
// REPLACE your Orb.cpp with this file. Changes from lesson 20 are marked NEW.

#include "Orb.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"

AOrb::AOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	// An invisible root, so the orb has a place in the world (lesson 22
	// replaces this with a real mesh).
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AOrb::BeginPlay()
{
	Super::BeginPlay();

	// NEW (lesson 21): log the nickname and the speed this orb really has.
	//
	// Why the * in *Nickname? %s wants raw characters (a TCHAR pointer), the
	// way C's printf does. An FString is an object, not raw characters.
	// The * operator on an FString hands over its raw characters.
	// Rule: an FString going into a %s always gets a * in front.
	UE_LOG(LogTemp, Warning, TEXT("%s (%s) spins at %f degrees per second"),
		*GetName(), *Nickname, RotationSpeed);
}

void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// NEW (lesson 21): the hard-coded 90.f became the RotationSpeed property,
	// so every orb can spin at its own speed.
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(40.f, 40.f, 40.f),
		GetActorQuat(), FColor::Yellow);
}
