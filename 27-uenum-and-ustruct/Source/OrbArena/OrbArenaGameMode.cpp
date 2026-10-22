// OrbArenaGameMode.cpp - Lesson 27: Typed, data-driven orbs
//
// Template file. Keep your constructor's original lines. Changes from lesson
// 26 are marked NEW.

#include "OrbArenaGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Orb.h"

// NEW (lesson 27): a small helper that builds one FOrbKindInfo.
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

	// NEW (lesson 27): default settings for each kind of orb. These are only
	// the starting values: tune them in the GameMode Blueprint's Class Defaults.
	OrbKinds.Add(EOrbKind::Common, MakeKindInfo(1, 1.f));
	OrbKinds.Add(EOrbKind::Rare, MakeKindInfo(5, 0.7f));
	OrbKinds.Add(EOrbKind::Golden, MakeKindInfo(20, 1.6f));
}

void AOrbArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));

	TArray<AActor*> FoundOrbs;
	UGameplayStatics::GetAllActorsOfClass(this, AOrb::StaticClass(), FoundOrbs);
	OrbsLeft = FoundOrbs.Num();
	UE_LOG(LogTemp, Warning, TEXT("%d orbs to collect. Go!"), OrbsLeft);
}

void AOrbArenaGameMode::AddScore(int32 Points)
{
	Score += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

// NEW (lesson 27) ------------------------------------------------------------
const FOrbKindInfo* AOrbArenaGameMode::FindKindInfo(EOrbKind Kind) const
{
	// Find on a const TMap gives back a const pointer: look, do not touch.
	return OrbKinds.Find(Kind);
}

// NEW (lesson 27): same shape as lesson 26, but with an enum key and a struct
// value instead of a name and a number.
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
