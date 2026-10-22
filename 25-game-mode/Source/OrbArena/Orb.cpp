// Orb.cpp - Lesson 25: The referee
//
// REPLACE your Orb.cpp with this file. Changes from lesson 24 are marked NEW.
// Orb.h does not change in this lesson.

#include "Orb.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"                   // NEW (lesson 25): GetWorld()->...
#include "GameFramework/Character.h"
#include "OrbArenaGameMode.h"               // NEW (lesson 25): the referee
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

	// NEW (lesson 25) --------------------------------------------------------
	//
	// Ask the world for its GameMode, as our own class.
	// GetAuthGameMode<T>() returns the level's GameMode cast to T: a pointer to
	// it if the level really uses an AOrbArenaGameMode (or a Blueprint child of
	// it), or nullptr if the level uses some other GameMode.
	// ("Auth" means "authority": in a multiplayer game only the server has the
	// GameMode. In a single-player game, you always do.)
	AOrbArenaGameMode* GameMode = GetWorld()->GetAuthGameMode<AOrbArenaGameMode>();

	// Lesson 13 again: never use a pointer that might be empty.
	if (GameMode != nullptr)
	{
		GameMode->AddScore(1);
	}
	else
	{
		// Say clearly what is wrong, for whoever reads the log (probably you).
		UE_LOG(LogTemp, Error,
			TEXT("This level does not use AOrbArenaGameMode, so nobody is keeping score."));
	}

	Destroy();
}
