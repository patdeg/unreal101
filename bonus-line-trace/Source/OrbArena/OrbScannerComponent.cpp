// OrbScannerComponent.cpp - Bonus: line traces
//
// NEW FILE. Replace the wizard's OrbScannerComponent.cpp with this.

#include "OrbScannerComponent.h"

#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Orb.h"

UOrbScannerComponent::UOrbScannerComponent()
{
	// Components, like Actors, only tick if you ask.
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbScannerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// The component lives on the player's character. GetOwner() is that actor.
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return;
	}

	// Only a player has a camera to look through.
	APlayerController* PlayerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (PlayerController == nullptr)
	{
		return;
	}

	// Where is the camera, and which way does it look? GetPlayerViewPoint
	// fills two variables we pass by reference (lesson 10).
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// The ray: from the camera, straight ahead (the center of the screen).
	// CameraRotation.Vector() is a direction of length 1.
	const FVector Start = CameraLocation;
	const FVector End = Start + CameraRotation.Vector() * ScanDistance;

	// Do not hit ourselves. The camera sits behind the character, so the ray
	// would otherwise pass straight through our own body first.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerPawn);

	// The line trace. It fills Hit (by reference) and returns true if the ray
	// hit something that BLOCKS the Visibility channel.
	FHitResult Hit;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End,
		ECC_Visibility, Params);

	if (bDrawScanLine)
	{
		// Red line to where it hit, or to its full length if it hit nothing.
		// (ImpactPoint is a special kind of FVector; FVector(...) makes both
		// sides of the ?: the same type.)
		const FVector LineEnd = bHit ? FVector(Hit.ImpactPoint) : End;

		// For DRAWING only, start the line at the character, not the camera.
		// The real ray starts at the camera and runs exactly along your view,
		// so you would see it end-on: a dot, not a line. Drawn from the
		// character, it shows up as a line from you to what the scanner sees.
		DrawDebugLine(GetWorld(), OwnerPawn->GetActorLocation(), LineEnd, FColor::Red);
	}

	if (!bHit)
	{
		return;
	}

	// What did we hit? Hit.GetActor() is the actor, or nullptr. Is it an orb?
	// Cast, lesson 24.
	AOrb* Orb = Cast<AOrb>(Hit.GetActor());
	if (Orb != nullptr)
	{
		// Highlight it with a cyan sphere around it, for this frame.
		DrawDebugSphere(GetWorld(), Orb->GetActorLocation(), 80.f, 16, FColor::Cyan);
	}
}
