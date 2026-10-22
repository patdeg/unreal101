// OrbArenaGameMode.cpp - Lesson 26: Unreal's containers
//
// Template file. Keep your constructor's original lines; ADD the lines marked NEW.

#include "OrbArenaGameMode.h"

#include "Kismet/GameplayStatics.h"   // NEW (lesson 26): GetAllActorsOfClass
#include "Orb.h"                      // NEW (lesson 26): AOrb

AOrbArenaGameMode::AOrbArenaGameMode()
{
	// Keep whatever your template has in here, and add:

	// NEW (lesson 26): default points for each orb type. Like lesson 07's
	// prices["sword"] = 50, but with Add(key, value).
	PointsTable.Add(FName(TEXT("Common")), 1);
	PointsTable.Add(FName(TEXT("Rare")), 5);
	PointsTable.Add(FName(TEXT("Golden")), 20);
}

void AOrbArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));

	// NEW (lesson 26) --------------------------------------------------------
	//
	// TArray<T> is Unreal's std::vector (lesson 05).
	// GetAllActorsOfClass fills the array with every actor of that class in
	// the level. It fills OUR array, so it takes it by reference (lesson 10).
	//
	// AOrb::StaticClass() is "the class AOrb, as a value you can pass around".
	TArray<AActor*> FoundOrbs;
	UGameplayStatics::GetAllActorsOfClass(this, AOrb::StaticClass(), FoundOrbs);

	// Num() is TArray's size(). Range-for works the same as with std::vector.
	OrbsLeft = FoundOrbs.Num();
	for (AActor* Orb : FoundOrbs)
	{
		UE_LOG(LogTemp, Log, TEXT("Found %s"), *Orb->GetName());
	}
	UE_LOG(LogTemp, Warning, TEXT("%d orbs to collect. Go!"), OrbsLeft);
}

void AOrbArenaGameMode::AddScore(int32 Points)
{
	Score += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

// NEW (lesson 26) ------------------------------------------------------------
void AOrbArenaGameMode::OrbCollected(FName OrbType)
{
	// TMap::Find returns a POINTER to the value: the address of the points if
	// the key exists, or nullptr if it does not.
	//
	// Compare lesson 07: std::map's prices["sowrd"] silently INSERTED the
	// typo with value 0. Find never inserts anything. It answers "here it is"
	// or "not found", and you must check which (lesson 13).
	const int32* Points = PointsTable.Find(OrbType);

	if (Points != nullptr)
	{
		AddScore(*Points);   // * reads the value the pointer points at
	}
	else
	{
		// FName -> text for the log: ToString() gives an FString, * gives %s
		// its raw characters (lesson 21).
		UE_LOG(LogTemp, Error, TEXT("No points for orb type '%s'. Check the PointsTable."),
			*OrbType.ToString());
	}

	OrbsLeft--;
	if (OrbsLeft <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("You win! Final score: %d"), Score);
	}
}
