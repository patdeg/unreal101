// Enemy.cpp - Lesson 30: Something chases you
//
// NEW FILE. Replace the wizard's Enemy.cpp with this.

#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"   // GetPlayerPawn
#include "UObject/ConstructorHelpers.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// A Character only moves if something CONTROLS it: the player's
	// controller, or an AI controller. This line says: "when I am placed in the
	// level or spawned, give me an AI controller automatically."
	// What you can ignore for now: the AI controller itself. We do not use any
	// of its clever features; it just has to exist so movement works.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// A box body, attached to the capsule (the Character's root component).
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Body->SetStaticMesh(CubeMesh.Object);
	}

	// The engine cube is 100 cm on each side. Make it a tall block, roughly
	// the size of the capsule.
	Body->SetRelativeScale3D(FVector(0.6f, 0.6f, 1.6f));

	// The capsule already handles collision. The box is only for looks.
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Turn to face the direction we walk, instead of the controller's aim.
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Apply the editor-tunable speed to the movement component.
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Who is the player? Player number 0 is the first (and only) player.
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (Player == nullptr)
	{
		return;   // no player (yet): nothing to chase
	}

	// The vector math of the chase, in three steps:
	//
	// 1. "Where is the player, seen from me?"
	//    Target minus Me gives an arrow from me to the target.
	//    The ORDER matters: B - A points from A to B.
	const FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();

	// 2. Ignore height, so the enemy does not try to walk into the sky.
	FVector Direction = ToPlayer;
	Direction.Z = 0.f;

	// 3. Make it length 1 ("normalize"), so it is a pure direction, not a
	//    distance. GetSafeNormal returns a zero vector instead of dividing by
	//    zero when we are standing exactly on the player.
	Direction = Direction.GetSafeNormal();

	// "I want to walk this way." The movement component does the rest:
	// speed, acceleration, gravity, slopes. Same function the player's input
	// calls when you press W.
	AddMovementInput(Direction, 1.f);

	// Size() is the length of the arrow: the distance to the player in cm.
	if (!bHasCaughtPlayer && ToPlayer.Size() < CatchDistance)
	{
		bHasCaughtPlayer = true;
		UE_LOG(LogTemp, Warning, TEXT("Caught! %s got you."), *GetName());
	}
}
