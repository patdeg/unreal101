// OrbHUDWidget.h - Lesson 34: Capstone, Orb Arena 1.0
//
// REPLACE your OrbHUDWidget.h with this file. Changes from lesson 33 are
// marked NEW. WBP_HUD must now also contain two more Text widgets, named
// exactly TimeText and GameOverText (BindWidget, lesson 33).

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrbHUDWidget.generated.h"   // always the last include

class UTextBlock;

UCLASS()
class ORBARENA_API UOrbHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetScore(int32 NewScore);

	// NEW (milestone 4): listens to OnTimeChanged (one int32).
	UFUNCTION()
	void SetTime(int32 SecondsLeft);

	// NEW (milestone 4): listens to OnGameOver (one bool).
	UFUNCTION()
	void ShowGameOver(bool bWon);

protected:
	// NEW (milestone 4): runs when the widget is created and shown. Our chance
	// to hide the game-over text at the start. (Super call required, as with
	// BeginPlay.)
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;

	// NEW (milestone 4)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameOverText;
};
