// OrbScannerComponent.h - Bonus: line traces
//
// NEW FILE. Create it with Tools > New C++ Class, choose "Actor Component" as
// the parent, and name it "OrbScannerComponent". Then replace the wizard's code
// with this.
//
// A component, not an Actor: it is a PART you add to the player (lesson 22),
// so you do not need to touch the template's character code at all.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbScannerComponent.generated.h"   // always the last include

// BlueprintSpawnableComponent: lets you add this component to a Blueprint
// with the "+ Add" button. ClassGroup puts it under "Custom" in that list.
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBARENA_API UOrbScannerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbScannerComponent();

	// Components tick too, with a slightly longer signature than Actors.
	// You only use DeltaTime; the other two parameters are engine details.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// How far the scan reaches, in cm.
	UPROPERTY(EditAnywhere, Category = "Scanner", meta = (ClampMin = "0"))
	float ScanDistance = 3000.f;

	// Draw the scan line, to see what the trace is doing.
	UPROPERTY(EditAnywhere, Category = "Scanner")
	bool bDrawScanLine = true;
};
