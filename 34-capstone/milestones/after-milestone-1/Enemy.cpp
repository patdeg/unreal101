// Enemy.cpp - Lesson 34, AFTER MILESTONE 1 (enemy states)
//
// A snapshot: this is how Enemy.cpp looks at the end of milestone 1. It needs
// nothing from later milestones, so it builds with lesson 33's GameMode.
// Changes from lesson 30 are marked NEW (milestone 1).
//
// Milestone 3 changes it again (the round-over freeze and PlayerCaught): see
// the finished version in ../../Source/OrbArena/Enemy.cpp.

#include "Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// NEW (milestone 1): the box body's normal size and its squashed "stunned"
// size. The squashed box is also moved down, so it sits near the floor
// instead of hanging in the middle of the capsule.
static const FVector NormalBodyScale(0.6f, 0.6f, 1.6f);
static const FVector StunnedBodyScale(0.8f, 0.8f, 0.6f);
static const FVector StunnedBodyOffset(0.f, 0.f, -55.f);

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Body->SetStaticMesh(CubeMesh.Object);
	}
	Body->SetRelativeScale3D(NormalBodyScale);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetState(EEnemyState::Idle);   // NEW (milestone 1)
}

// NEW (milestone 1) ----------------------------------------------------------
void AEnemy::SetState(EEnemyState NewState)
{
	State = NewState;

	// Show the state: a squashed box near the floor while stunned, a tall one
	// otherwise.
	const bool bStunned = (State == EEnemyState::Stunned);
	Body->SetRelativeScale3D(bStunned ? StunnedBodyScale : NormalBodyScale);
	Body->SetRelativeLocation(bStunned ? StunnedBodyOffset : FVector::ZeroVector);

	UE_LOG(LogTemp, Log, TEXT("%s is now %s"), *GetName(), *UEnum::GetValueAsString(State));
}

// NEW (milestone 1) ----------------------------------------------------------
void AEnemy::Stun(float Seconds)
{
	StunTimeLeft = Seconds;
	GetCharacterMovement()->StopMovementImmediately();
	SetState(EEnemyState::Stunned);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (Player == nullptr)
	{
		return;
	}

	const FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	const double Distance = ToPlayer.Size();

	// NEW (milestone 1): the state machine. Lesson 09's switch, one case per
	// state. Each case does its own job AND decides whether to move to another
	// state.
	switch (State)
	{
	case EEnemyState::Idle:
		if (Distance < SightRange)
		{
			SetState(EEnemyState::Chase);
		}
		break;

	case EEnemyState::Chase:
	{
		// (Braces around this case: it declares a local variable.)
		FVector Direction = ToPlayer;
		Direction.Z = 0.f;
		AddMovementInput(Direction.GetSafeNormal(), 1.f);

		if (Distance < CatchDistance)
		{
			// Still lesson 30's "log it once". Milestone 3 turns this into
			// "tell the GameMode, and the round ends".
			if (!bHasCaughtPlayer)
			{
				bHasCaughtPlayer = true;
				UE_LOG(LogTemp, Warning, TEXT("Caught! %s got you."), *GetName());
			}
		}
		else if (Distance > SightRange * 1.5)
		{
			SetState(EEnemyState::Idle);   // lost sight of the player
		}
		break;
	}

	case EEnemyState::Stunned:
		StunTimeLeft -= DeltaTime;
		if (StunTimeLeft <= 0.f)
		{
			SetState(EEnemyState::Chase);
		}
		break;
	}
}
