// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "LevelManagerEnums.generated.h"

UENUM(BlueprintType)
enum class EPersistentLevel : uint8
{
	/** None value. */
	None = 0,

	/** App start map. */
	Menu = 1,

	/** Game map. */
	World = 2,

};
