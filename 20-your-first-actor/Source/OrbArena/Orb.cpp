// Orb.cpp - Lesson 20: Your first Actor
//
// NEW FILE. Replace the wizard's Orb.cpp with this one (or type the changes
// into it). Arena step: an orb that spins.

#include "Orb.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"   // DrawDebugBox, for drawing a quick wireframe

AOrb::AOrb()
{
	// Actors do NOT tick unless you ask. Ticking every frame costs a little
	// time, so Unreal makes you opt in. The wizard writes this line for you.
	PrimaryActorTick.bCanEverTick = true;

	// What you can ignore for now: this line gives the orb a "root", an
	// invisible point in the world that holds its position and rotation.
	// Without it the orb would have no place in the world at all.
	// Lesson 22 explains components properly and replaces this line.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AOrb::BeginPlay()
{
	// Always let the parent class do its setup first (lesson 19).
	Super::BeginPlay();

	// GetName() returns an FString (Unreal's std::string). The * in front
	// turns it into the raw text that %s expects. Lesson 21 explains why.
	UE_LOG(LogTemp, Warning, TEXT("An orb is ready: %s"), *GetName());
}

void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// How fast to spin, in degrees per SECOND (not per frame).
	const float DegreesPerSecond = 90.f;

	// Multiply by DeltaTime so the speed is the same on a fast PC (many short
	// frames) and a slow PC (fewer long frames). At 60 frames per second,
	// DeltaTime is about 0.0167, so each frame turns about 1.5 degrees.
	//
	// FRotator(Pitch, Yaw, Roll): Yaw turns around the up axis, like a top.
	AddActorLocalRotation(FRotator(0.f, DegreesPerSecond * DeltaTime, 0.f));

	// The orb has no mesh yet (lesson 22), so draw a yellow wireframe box
	// where it is, turned the same way it is, just for this one frame.
	// GetActorQuat() is the actor's rotation in a form DrawDebugBox wants.
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(40.f, 40.f, 40.f),
		GetActorQuat(), FColor::Yellow);
}
