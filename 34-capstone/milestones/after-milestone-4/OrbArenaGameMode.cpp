// OrbArenaGameMode.cpp - Lesson 34, AFTER MILESTONE 4 (HUD polish)
//
// A snapshot: how OrbArenaGameMode.cpp looks at the end of milestone 4.
// Template file: keep your constructor's original lines. Changes from
// lesson 33 are marked NEW, with the milestone that adds them.
//
// Milestone 5 adds RestartRound (and an include for it): see the finished
// version in ../../Source/OrbArena/OrbArenaGameMode.cpp.

#include "OrbArenaGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"   // NEW (milestone 3)
#include "OrbHUDWidget.h"
#include "TimerManager.h"                     // NEW (milestone 3)

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
	OrbKinds.Add(EOrbKind::Common, MakeKindInfo(1, 1.f));
	OrbKinds.Add(EOrbKind::Rare, MakeKindInfo(5, 0.7f));
	OrbKinds.Add(EOrbKind::Golden, MakeKindInfo(20, 1.6f));
}

void AOrbArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));

	// NEW (milestone 3): start the round clock, ticking once per second.
	TimeLeft = RoundSeconds;
	GetWorldTimerManager().SetTimer(RoundTimer, this, &AOrbArenaGameMode::TickRound, 1.f, true);

	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("HUD Widget Class is empty: no HUD. Set it to WBP_HUD in the GameMode Blueprint."));
		return;
	}

	UOrbHUDWidget* Widget = CreateWidget<UOrbHUDWidget>(GetWorld(), HUDWidgetClass);
	if (Widget == nullptr)
	{
		return;
	}
	Widget->AddToViewport();
	Widget->SetScore(Score);
	Widget->SetTime(TimeLeft);   // NEW (milestone 4)

	OnScoreChanged.AddDynamic(Widget, &UOrbHUDWidget::SetScore);
	OnTimeChanged.AddDynamic(Widget, &UOrbHUDWidget::SetTime);         // NEW (milestone 4)
	OnGameOver.AddDynamic(Widget, &UOrbHUDWidget::ShowGameOver);       // NEW (milestone 4)

	HUDWidget = Widget;
}

void AOrbArenaGameMode::AddScore(int32 Points)
{
	Score += Points;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
	OnScoreChanged.Broadcast(Score);
}

const FOrbKindInfo* AOrbArenaGameMode::FindKindInfo(EOrbKind Kind) const
{
	return OrbKinds.Find(Kind);
}

void AOrbArenaGameMode::RegisterOrb()
{
	OrbsLeft++;
}

void AOrbArenaGameMode::OrbCollected(EOrbKind Kind)
{
	// NEW (milestone 3): after the final whistle, nothing counts.
	if (bRoundOver)
	{
		return;
	}

	const FOrbKindInfo* Info = FindKindInfo(Kind);
	if (Info != nullptr)
	{
		AddScore(Info->Points);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No settings for %s. Check OrbKinds in the GameMode."),
			*UEnum::GetValueAsString(Kind));
	}

	OrbsLeft--;
	CheckForWin();   // NEW (milestone 3): replaces the old "OrbsLeft <= 0" log
}

// NEW (milestone 3) ----------------------------------------------------------

void AOrbArenaGameMode::RegisterSpawner()
{
	SpawnersRunning++;
}

void AOrbArenaGameMode::SpawnerFinished()
{
	SpawnersRunning--;
	CheckForWin();
}

void AOrbArenaGameMode::CheckForWin()
{
	// Win only when no more orbs can appear AND none are left.
	if (SpawnersRunning <= 0 && OrbsLeft <= 0)
	{
		EndRound(true);
	}
}

void AOrbArenaGameMode::PlayerCaught()
{
	EndRound(false);
}

void AOrbArenaGameMode::TickRound()
{
	TimeLeft--;
	OnTimeChanged.Broadcast(TimeLeft);

	if (TimeLeft <= 0)
	{
		EndRound(false);   // out of time
	}
}

void AOrbArenaGameMode::EndRound(bool bWon)
{
	// Several things can try to end the round in the same moment (the clock
	// and an enemy, say). Only the first one counts.
	if (bRoundOver)
	{
		return;
	}
	bRoundOver = true;

	GetWorldTimerManager().ClearTimer(RoundTimer);

	// Stop the player from walking. (Looking around and the restart key still
	// work: we only ignore MOVE input.)
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetIgnoreMoveInput(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Round over: %s. Final score: %d"),
		bWon ? TEXT("you win") : TEXT("you lose"), Score);

	OnGameOver.Broadcast(bWon);
}
