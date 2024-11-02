// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Data/DataAsset/PersistentLevelManagerConfig.h"
#include "Data/DataAsset/GeneralGameplayConfig.h"

#include "RootConfiguration.generated.h"

/**
 * 
 */
UCLASS()
class MEP_API URootConfiguration : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Default)
	bool EnableEditorOnScreenMessages;

	UPROPERTY(EditAnywhere, Category = Default)
	UPersistentLevelManagerConfig* PersistentLevelManagerConfig;

	UPROPERTY(EditAnywhere, Category = Default)
	UGeneralGameplayConfig* GeneralGameplayConfigEditor;

	UPROPERTY(EditAnywhere, Category = Default)
	UGeneralGameplayConfig* GeneralGameplayConfigProduction;

public:

	UFUNCTION(BlueprintPure, Category = "Config")
	UGeneralGameplayConfig* GetGeneralGameplayConfig() const;
};
