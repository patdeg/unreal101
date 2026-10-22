// Orb.cpp - Lesson 22: Actors are built from parts
//
// REPLACE your Orb.cpp with this file. Changes from lesson 21 are marked NEW.
// The invisible "Root" and the debug box are gone: the mesh replaces both.

#include "Orb.h"

// NEW (lesson 22): the full declarations of the types we now really use.
// Orb.h only forward-declared UStaticMeshComponent. Here we call its
// functions, so the compiler needs the whole class (lesson 14).
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AOrb::AOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	// NEW (lesson 22) --------------------------------------------------------
	//
	// CreateDefaultSubobject<T>(name) creates a component that belongs to this
	// actor. The <UStaticMeshComponent> is a template argument. Read it like
	// std::vector<int>: "make me one of THESE".
	// The name in TEXT() must be unique inside this actor. It is the name you
	// see in the editor's component list.
	//
	// Who deletes it? Not you. Unreal's garbage collector owns it (lesson 28).
	// No unique_ptr, no delete.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// The root component is the one that holds the actor's position and
	// rotation. Everything else attaches to it. Our mesh is the root.
	RootComponent = Mesh;

	// Load the sphere mesh that ships with the engine. The text is a path to
	// an asset: "/Engine/..." means the engine's own content folder.
	// "static" means this lookup runs once, not once per orb.
	// What you can ignore for now: ConstructorHelpers only works inside a
	// constructor, and the path format is Folder/AssetName.AssetName.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	// Lesson 13's rule: a lookup can fail, so check before you use it.
	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
	}

	// The engine sphere is 100 cm across. Make it half that size.
	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	// Let the player walk through orbs instead of bumping into them.
	// Lesson 24 adds a separate "trigger" for touching.
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AOrb::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%s (%s) spins at %f degrees per second"),
		*GetName(), *Nickname, RotationSpeed);
}

void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotating the actor rotates its root component, and with it every
	// component attached to the root.
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}
