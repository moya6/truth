// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundBase.h"

#include "GeneralGameplayConfig.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MEP_API UGeneralGameplayConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UGeneralGameplayConfig();

public:

	/* Game */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	bool SkipMainMenu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	bool UseVhsFilter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game", 
		meta = (
			ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", 
			ToolTip = "Base speed for subtitle text write, the bigger value the longer it takes to write the text, 0.0 means text is written instantly"))
	float SubtitleTextWriteTimeBase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game",
		meta = (
			ClampMin = "0.0", ClampMax = "5.0", UIMin = "0.0", UIMax = "5.0",
			ToolTip = "Base display time of subtitle text, 0.0 means the text does not dissapear until in is replaced by something else"))
	float SubtitleTextDisplayTimeBase;

	/* Player */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	float PlayerMovementSpeedBase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player",
		meta = (
			ClampMin = "40.0", ClampMax = "120.0", UIMin = "40.0", UIMax = "120.0"))
	float PlayerFovBase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Interaction")
	bool InteractionTraceDebug;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Interaction")
	float InteractionTraceRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Interaction")
	float InteractionTraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	float PlayerMovementSpeedZoomedBase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Camera",
		meta = (
			ClampMin = "40.0", ClampMax = "120.0", UIMin = "40.0", UIMax = "120.0"))
	float PlayerFovZoomed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Camera",
		meta = (
			ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float PlayerZoomSpeed;

	/* AI */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool NpcDebugModeEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float NpcInvestigationDelayDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float NpcAttackDelayDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FVector NpcSightConeShapeDefault;

	/* Settings */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<FString> SupportedResolutionList;

	/* Vehicle */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USoundBase* VehicleDoorOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USoundBase* VehicleDoorCloseSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USoundBase* VehicleTrunkOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USoundBase* VehicleTrunkCloseSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USoundBase* VehicleEngineStartSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USoundBase* VehicleEngineStopSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	USoundBase* VehicleEngineLoopSound;

	/* Game Progress */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Progress")
	FName StartingPlayerLocationTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Progress")
	TArray<FName> MilestonesUnlockedAtStart;
};
