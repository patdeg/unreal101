// OrbArenaGameMode.cpp - Lesson 25: The referee
//
// Template file. Keep your constructor's body; ADD the lines marked NEW.

#include "OrbArenaGameMode.h"

AOrbArenaGameMode::AOrbArenaGameMode()
{
	// Keep whatever your template has in here.
}

void AOrbArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));
}

// NEW (lesson 25) ------------------------------------------------------------
//
// The one and only place where the score changes. If the score is ever
// wrong, there is exactly one function to put a breakpoint in.
void AOrbArenaGameMode::AddScore(int32 Points)
{
	Score += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}
