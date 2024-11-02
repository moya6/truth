// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Data/Enum/LevelManagerEnums.h"
#include "Data/DataAsset/PersistentLevelConfig.h"

#include "PersistentLevelManagerConfig.generated.h"

/**
 * 
 */
UCLASS()
class MEP_API UPersistentLevelManagerConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Default)
	TMap<EPersistentLevel, UPersistentLevelConfig*> PersistentLevelConfigMap;
	
};
