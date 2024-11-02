// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "UserInterfaceEnums.generated.h"

UENUM(BlueprintType)
enum class EUserInterfaceMode : uint8
{
	/** loading screen */
	Loading = 0,

	/** splash screen */
	Splash = 1,

	/** player hud */
	Player = 2,

	/** main menu */
	MainMenu = 3,

	/** pause menu */
	PauseMenu = 4,

	/** cutscene */
	Cutscene = 5,

};
