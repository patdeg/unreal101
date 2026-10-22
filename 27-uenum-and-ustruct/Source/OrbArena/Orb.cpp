// Orb.cpp - Lesson 27: Typed, data-driven orbs
//
// REPLACE your Orb.cpp with this file. Changes from lesson 26 are marked NEW.

#include "Orb.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
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

	ApplyKind();   // NEW (lesson 27)
}

// NEW (lesson 27) ------------------------------------------------------------
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
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}

	AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();
	if (GameMode != nullptr)
	{
		GameMode->OrbCollected(Kind);   // NEW (lesson 27): the enum, not a name
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("This level does not use AOrbArenaGameMode, so nobody is keeping score."));
	}

	Destroy();
}
