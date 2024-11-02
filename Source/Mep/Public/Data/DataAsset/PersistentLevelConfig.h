// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PersistentLevelConfig.generated.h"

/**
 * 
 */
UCLASS()
class MEP_API UPersistentLevelConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString PersistentLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> BaseStreamingLevelList;
	
};
