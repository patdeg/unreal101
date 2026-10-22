// OrbHUDWidget.h - Lesson 33: A HUD on screen
//
// NEW FILE. Create it with Tools > New C++ Class. In the wizard, choose
// "All Classes", search for "UserWidget", pick it as the parent, and name the
// class "OrbHUDWidget". Then replace the wizard's code with this.
//
// This class holds the LOGIC of the HUD. The LOOK (where the text sits, its
// font and color) is drawn in a Widget Blueprint that inherits from it.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrbHUDWidget.generated.h"   // always the last include

class UTextBlock;

// U prefix: a widget is an Unreal object, but not an Actor (it does not live
// in the 3D level, it lives on the screen).
UCLASS()
class ORBARENA_API UOrbHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Updates the score text. It is a UFUNCTION because the GameMode's
	// OnScoreChanged delegate (lesson 31) will call it with AddDynamic, and
	// its parameter (one int32) matches that delegate exactly.
	UFUNCTION()
	void SetScore(int32 NewScore);

protected:
	// BindWidget: "in the Widget Blueprint, there MUST be a widget named
	// exactly ScoreText, of type Text Block. Fill this pointer with it."
	//
	// It is like document.getElementById("ScoreText") in JavaScript, except it
	// is checked when the Widget Blueprint compiles, not when the game runs.
	// If the name or type does not match, the Blueprint refuses to compile.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
};
