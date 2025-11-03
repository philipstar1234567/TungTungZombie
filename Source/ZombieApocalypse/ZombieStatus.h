#pragma once

#include "CoreMinimal.h"
#include "ZombieStatus.generated.h"


// An enum for the current status of the human.
UENUM(BlueprintType)
enum class EZombieState : uint8
{
	Human UMETA(DisplayName = "Human"),
	Bitten UMETA(DisplayName = "Bitten"),
	Zombie UMETA(DisplayName = "Zombie")
};

