// OrbHUDWidget.cpp - Lesson 34: Capstone, Orb Arena 1.0
//
// REPLACE your OrbHUDWidget.cpp with this file. Changes from lesson 33 are
// marked NEW.

#include "OrbHUDWidget.h"

#include "Components/TextBlock.h"

// NEW (milestone 4) ----------------------------------------------------------
void UOrbHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Hidden until the round ends. Collapsed = invisible AND takes no space.
	if (GameOverText != nullptr)
	{
		GameOverText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOrbHUDWidget::SetScore(int32 NewScore)
{
	if (ScoreText == nullptr)
	{
		return;
	}
	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), NewScore)));
}

// NEW (milestone 4) ----------------------------------------------------------
void UOrbHUDWidget::SetTime(int32 SecondsLeft)
{
	if (TimeText == nullptr)
	{
		return;
	}

	// 75 seconds -> "1:15". Integer division and remainder (Part 1, lesson 02).
	// %02d means "at least 2 digits, pad with 0", so 5 seconds shows as "05".
	const int32 Minutes = SecondsLeft / 60;
	const int32 Seconds = SecondsLeft % 60;
	TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %d:%02d"), Minutes, Seconds)));
}

// NEW (milestone 4) ----------------------------------------------------------
void UOrbHUDWidget::ShowGameOver(bool bWon)
{
	if (GameOverText == nullptr)
	{
		return;
	}

	const TCHAR* Message = bWon
		? TEXT("You win! Press R to play again.")
		: TEXT("Game over. Press R to try again.");

	GameOverText->SetText(FText::FromString(Message));
	GameOverText->SetVisibility(ESlateVisibility::Visible);
}
