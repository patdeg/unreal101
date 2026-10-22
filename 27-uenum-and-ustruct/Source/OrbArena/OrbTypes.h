// OrbTypes.h - Lesson 27: Typed, data-driven orbs
//
// NEW FILE. A header can hold just types; it does not need a .cpp partner.
//
// Create it with File Explorer, in Source/OrbArena/ next to Orb.h (README
// step 2 has the details). Turn on View > Show > File name extensions first,
// or you may end up with OrbTypes.h.txt. Do NOT use Visual Studio's
// Add > New Item: it puts the file in a generated folder (Intermediate\...)
// where Unreal never looks. Then right-click OrbArena.uproject > Generate
// Visual Studio project files, so Visual Studio shows it.

#pragma once

#include "CoreMinimal.h"
#include "OrbTypes.generated.h"   // UHT's generated code: always the last include

// ----------------------------------------------------------------------------
// A UENUM is lesson 09's enum class, plus a marker so the editor can see it.
//
//   BlueprintType - the editor and Blueprints may use this type (it shows up
//                   as a dropdown in the Details panel).
//   : uint8       - store it in one byte (values 0 to 255). Unreal requires
//                   this for enums that Blueprints can use.
//
// The "E" prefix is Unreal's naming rule for enums.
// ----------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EOrbKind : uint8
{
	Common,
	Rare,
	Golden
};

// ----------------------------------------------------------------------------
// A USTRUCT is lesson 08's struct, plus markers so the editor can see it.
//
// It needs GENERATED_BODY() as its first line, just like a UCLASS.
// Every member you want the editor to show needs its own UPROPERTY.
//
// The "F" prefix is Unreal's naming rule for structs (think "fixed shape").
// ----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FOrbKindInfo
{
	GENERATED_BODY()

	// How many points an orb of this kind is worth.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb")
	int32 Points = 1;

	// How big an orb of this kind is: 1 is normal, 2 is twice the size.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Orb")
	float Scale = 1.f;
};
