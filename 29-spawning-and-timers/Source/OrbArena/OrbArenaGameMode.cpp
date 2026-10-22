// OrbArenaGameMode.cpp - Lesson 29: Things appear over time
//
// Template file. Keep your constructor's original lines. Changes from lesson
// 27 are marked NEW.

#include "OrbArenaGameMode.h"

// A small helper that builds one FOrbKindInfo (lesson 27).
// "static" here means: only this .cpp file can see this function.
static FOrbKindInfo MakeKindInfo(int32 Points, float Scale)
{
	FOrbKindInfo Info;
	Info.Points = Points;
	Info.Scale = Scale;
	return Info;
}

AOrbArenaGameMode::AOrbArenaGameMode()
{
	// Keep whatever your template has in here, and add:

	// Default settings for each kind of orb (lesson 27).
	OrbKinds.Add(EOrbKind::Common, MakeKindInfo(1, 1.f));
	OrbKinds.Add(EOrbKind::Rare, MakeKindInfo(5, 0.7f));
	OrbKinds.Add(EOrbKind::Golden, MakeKindInfo(20, 1.6f));
}

void AOrbArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));

	// NEW (lesson 29): the GetAllActorsOfClass count from lesson 26 is gone.
	// Why? Spawned orbs appear later, and Unreal does not promise WHICH actor's
	// BeginPlay runs first. If a spawner went first, its orbs would be counted
	// twice. Now each orb registers itself (see RegisterOrb), which works in
	// any order.
}

void AOrbArenaGameMode::AddScore(int32 Points)
{
	Score += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

const FOrbKindInfo* AOrbArenaGameMode::FindKindInfo(EOrbKind Kind) const
{
	// Find on a const TMap gives back a const pointer: look, do not touch.
	return OrbKinds.Find(Kind);
}

// NEW (lesson 29) ------------------------------------------------------------
void AOrbArenaGameMode::RegisterOrb()
{
	OrbsLeft++;
}

void AOrbArenaGameMode::OrbCollected(EOrbKind Kind)
{
	const FOrbKindInfo* Info = FindKindInfo(Kind);
	if (Info != nullptr)
	{
		AddScore(Info->Points);   // -> reads a member through a pointer (lesson 13)
	}
	else
	{
		// UEnum::GetValueAsString turns an enum value into readable text such as
		// "EOrbKind::Golden". Handy for logs.
		UE_LOG(LogTemp, Error, TEXT("No settings for %s. Check OrbKinds in the GameMode."),
			*UEnum::GetValueAsString(Kind));
	}

	OrbsLeft--;
	if (OrbsLeft <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("You win! Final score: %d"), Score);
	}
}
