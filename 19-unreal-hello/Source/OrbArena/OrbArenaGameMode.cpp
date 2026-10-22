// OrbArenaGameMode.cpp - Lesson 19: Unreal says hello
//
// Template file. DO NOT replace your copy: ADD the lines marked NEW.

#include "OrbArenaGameMode.h"

AOrbArenaGameMode::AOrbArenaGameMode()
{
	// Keep whatever your template has in here. Depending on the engine
	// version it may be empty, or it may set up which character the player
	// controls. Do not delete those lines.
}

// NEW (lesson 19) ------------------------------------------------------------
//
// "AOrbArenaGameMode::" means "this is the BeginPlay that belongs to the
// AOrbArenaGameMode class", the same as Fighter::takeDamage in lesson 14.
void AOrbArenaGameMode::BeginPlay()
{
	// Run the parent class's BeginPlay first. Super is Unreal's nickname for
	// "my parent class", here AGameModeBase. In lesson 15 you wrote
	// Monster::takeTurn(party) for the same idea.
	// Forgetting this line is a classic Unreal bug: the engine's own setup
	// for this object never runs.
	Super::BeginPlay();

	// UE_LOG is Unreal's console.log. Its three parts:
	//   LogTemp  - a category, so you can filter the Output Log
	//   Warning  - how loud: Log (grey), Warning (yellow), Error (red)
	//   TEXT()   - wraps a string literal so it uses Unreal's text type.
	//              Always put TEXT() around "..." you pass to Unreal.
	UE_LOG(LogTemp, Warning, TEXT("Orb Arena is running!"));
}
