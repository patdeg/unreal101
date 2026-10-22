// OrbHUDWidget.cpp - Lesson 33: A HUD on screen
//
// NEW FILE. Replace the wizard's OrbHUDWidget.cpp with this.

#include "OrbHUDWidget.h"

#include "Components/TextBlock.h"

void UOrbHUDWidget::SetScore(int32 NewScore)
{
	// BindWidget fills ScoreText when the widget is created. Checking it is
	// still a good habit (lesson 13): a check costs nothing, a crash costs a lot.
	if (ScoreText == nullptr)
	{
		return;
	}

	// Text the PLAYER reads is an FText (lesson 21's third text type), because
	// FText can be translated into other languages. Here we build an FString
	// with Printf and convert it. Fine for learning; a shipped game would use
	// FText::Format so translators can change the wording.
	ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), NewScore)));
}
