// Orb.cpp - Lesson 24: Something touched me
//
// REPLACE your Orb.cpp with this file. Changes from lesson 22 are marked NEW.

#include "Orb.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"     // NEW (lesson 24)
#include "Engine/StaticMesh.h"
#include "GameFramework/Character.h"        // NEW (lesson 24): ACharacter
#include "UObject/ConstructorHelpers.h"

AOrb::AOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// NEW (lesson 24) --------------------------------------------------------
	//
	// The trigger: an invisible sphere, attached to the mesh so it moves with it.
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Mesh);

	// Radius in cm, before scaling. The mesh is scaled by 0.5, and the trigger
	// is attached to it, so this ends up about 40 cm: a bit bigger than the
	// visible ball, so you collect it when you brush past.
	Trigger->SetSphereRadius(80.f);

	// A "collision profile" is a preset of collision settings.
	// "OverlapAllDynamic" means: never block anything, but notice (overlap)
	// anything that moves. Exactly what a pickup wants.
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Trigger->SetGenerateOverlapEvents(true);
}

void AOrb::BeginPlay()
{
	Super::BeginPlay();

	// NEW (lesson 24) --------------------------------------------------------
	//
	// "When something begins overlapping the Trigger, call HandleOverlap on
	// this orb." OnComponentBeginOverlap is an event (a delegate) that lives on
	// every collision component. AddDynamic adds a listener to it.
	// In JavaScript: trigger.addEventListener("beginoverlap", this.handleOverlap)
	//
	// We bind in BeginPlay, not in the constructor: the game has started and
	// the orb is fully set up.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOrb::HandleOverlap);

	UE_LOG(LogTemp, Warning, TEXT("%s (%s) is ready"), *GetName(), *Nickname);
}

void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

// NEW (lesson 24) ------------------------------------------------------------
void AOrb::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Anything can overlap the orb: the player, a falling crate, another orb.
	// Cast<ACharacter> asks: "is OtherActor really a character?"
	//   - yes: you get an ACharacter* pointing at it
	//   - no:  you get nullptr
	// It is the safe version of lesson 15's "is this Monster* really a Goblin?".
	// In JavaScript: if (other instanceof Character)
	ACharacter* Character = Cast<ACharacter>(OtherActor);

	// Lesson 13's rule: a pointer that can be empty must be checked.
	if (Character == nullptr)
	{
		return;   // not a character: ignore it
	}

	UE_LOG(LogTemp, Warning, TEXT("%s was collected by %s!"), *Nickname, *Character->GetName());

	// Remove this orb from the world. The engine cleans up the memory later
	// (lesson 28). After this line, do not use the orb any more.
	Destroy();
}
