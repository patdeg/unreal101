// OrbArenaGameMode.cpp - Lesson 33: A HUD on screen
//
// Template file. Keep your constructor's original lines. Changes from lesson
// 31 are marked NEW.

#include "OrbArenaGameMode.h"

#include "Blueprint/UserWidget.h"   // NEW (lesson 33): CreateWidget
#include "Engine/World.h"
#include "OrbHUDWidget.h"           // NEW (lesson 33)

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

	// NEW (lesson 33) --------------------------------------------------------
	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("HUD Widget Class is empty: no HUD. Set it to WBP_HUD in the GameMode Blueprint."));
		return;
	}

	// CreateWidget<T>(owner, class) builds one widget object from the class.
	// We create it in a plain LOCAL pointer first (fine inside one function,
	// lesson 28), wire it up, then keep it in the UPROPERTY member.
	UOrbHUDWidget* Widget = CreateWidget<UOrbHUDWidget>(GetWorld(), HUDWidgetClass);
	if (Widget == nullptr)
	{
		return;
	}

	// Put it on the screen. Like appending an element to the page's body.
	Widget->AddToViewport();

	// Show the starting score...
	Widget->SetScore(Score);

	// ...and subscribe it to every future change (lesson 31). The GameMode
	// does not call the HUD by hand anywhere: the event does it.
	OnScoreChanged.AddDynamic(Widget, &UOrbHUDWidget::SetScore);

	HUDWidget = Widget;
}

void AOrbArenaGameMode::AddScore(int32 Points)
{
	Score += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);

	// Tell every listener (lesson 31). The HUD is now one of them.
	OnScoreChanged.Broadcast(Score);
}

const FOrbKindInfo* AOrbArenaGameMode::FindKindInfo(EOrbKind Kind) const
{
	// Find on a const TMap gives back a const pointer: look, do not touch.
	return OrbKinds.Find(Kind);
}

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
