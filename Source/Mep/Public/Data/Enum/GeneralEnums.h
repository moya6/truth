// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GeneralEnums.generated.h"

UENUM(BlueprintType)
enum class EBinaryStateChange : uint8
{
	/** Change the state to ON. */
	TurnOn = 0,

	/** Change the state to OFF. */
	TurnOff = 1,

	/** Flip current state. */
	Flip = 2,
	
};
