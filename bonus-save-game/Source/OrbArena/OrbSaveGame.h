// OrbSaveGame.h - Bonus: saving
//
// NEW FILE. Create it with Tools > New C++ Class, choose "All Classes",
// search for "SaveGame", pick it as the parent, and name it "OrbSaveGame".
// Then replace the wizard's code with this. (No .cpp code is needed: the
// wizard's OrbSaveGame.cpp can stay as it is, with just its #include line.)
//
// A save game is a small object whose only job is to hold the data you want
// to keep between plays. Unreal can write it to a file and read it back.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OrbSaveGame.generated.h"   // always the last include

UCLASS()
class ORBARENA_API UOrbSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// UPROPERTY() is what makes this field SAVED. When Unreal writes a save
	// game to disk, it walks over the UPROPERTY members and writes each one.
	// A member without UPROPERTY() is invisible to that process: it is simply
	// not saved. (That is this lesson's "Break it".)
	UPROPERTY()
	int32 HighScore = 0;
};
