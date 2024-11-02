// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "NpcEnums.generated.h"


UENUM(BlueprintType)
enum class ENpcBehState : uint8
{
    /* when NPC is transitioning from current state to another state */
    Transition UMETA(DisplayName = "Transition"),
    
    /* target states */
    Idle UMETA(DisplayName = "Idle"),
    Work UMETA(DisplayName = "Work"),
    Investigate UMETA(DisplayName = "Investigate"),
    Attack UMETA(DisplayName = "Attack")

};

UENUM(BlueprintType)
enum class ENpcWorkPhase : uint8
{
    /* when NPC is not working */
    Rest     UMETA(DisplayName = "Rest"),

    /* when NPC is traveling to work */
    Commute  UMETA(DisplayName = "Commute"),

    /* when NPC is performing work */
    Work     UMETA(DisplayName = "Work")

};
