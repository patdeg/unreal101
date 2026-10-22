// Orb.cpp - Lesson 30: Something chases you
//
// REPLACE your Orb.cpp with this file. Changes from lesson 29 are marked NEW.
// Orb.h does not change in this lesson.

#include "Orb.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "OrbArenaCharacter.h"                // NEW (lesson 30): the player's class
#include "OrbArenaGameMode.h"
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

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Mesh);
	Trigger->SetSphereRadius(80.f);
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Trigger->SetGenerateOverlapEvents(true);
}

void AOrb::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOrb::HandleOverlap);

	// Every orb reports itself to the referee once (lesson 29).
	if (AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>())
	{
		GameMode->RegisterOrb();
	}

	ApplyKind();
}

// SpawnActor runs BeginPlay (and so ApplyKind) before the spawner picks a
// kind, so setting the kind applies it again (lesson 29).
void AOrb::SetKind(EOrbKind NewKind)
{
	Kind = NewKind;
	ApplyKind();
}

void AOrb::ApplyKind()
{
	AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();
	if (GameMode == nullptr)
	{
		return;
	}

	const FOrbKindInfo* Info = GameMode->FindKindInfo(Kind);
	if (Info == nullptr)
	{
		return;
	}

	// Resize the orb. Careful: Mesh is the root component, so its scale IS the
	// whole orb's size, and setting it REPLACES the old value, it does not
	// multiply it. So we multiply ourselves: 0.5 is the base size from the
	// constructor, and the kind's Scale makes it bigger or smaller.
	// A Golden orb (1.6) ends up 0.5 * 1.6 = 0.8. The Trigger is attached to
	// the Mesh, so it grows and shrinks with it.
	const float Size = 0.5f * Info->Scale;
	Mesh->SetRelativeScale3D(FVector(Size, Size, Size));
}

void AOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void AOrb::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// NEW (lesson 30) --------------------------------------------------------
	//
	// Before: Cast<ACharacter>. But the enemy is ALSO an ACharacter, so enemies
	// were collecting orbs! Cast to the PLAYER's class instead. Only an
	// AOrbArenaCharacter (or a Blueprint child of it, like the template's
	// BP_ThirdPersonCharacter) passes this check.
	AOrbArenaCharacter* Player = Cast<AOrbArenaCharacter>(OtherActor);
	if (Player == nullptr)
	{
		return;
	}

	AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();
	if (GameMode != nullptr)
	{
		GameMode->OrbCollected(Kind);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("This level does not use AOrbArenaGameMode, so nobody is keeping score."));
	}

	Destroy();
}
