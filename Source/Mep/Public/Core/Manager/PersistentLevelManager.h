// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/Enum/LevelManagerEnums.h"
#include "Data/DataAsset/PersistentLevelManagerConfig.h"

#include "PersistentLevelManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPersistentLevelInitialized);

UCLASS()
class MEP_API APersistentLevelManager : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Initialization")
	FOnPersistentLevelInitialized OnPersistentLevelInitialized;

public:	
	// Sets default values for this actor's properties
	APersistentLevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OpenPersistentLevel(EPersistentLevel level);

	UFUNCTION()
	EPersistentLevel GetCurrentPersistentLevel();

	UFUNCTION()
	UPersistentLevelConfig* GetPersistentLevelConfig(EPersistentLevel level);

	UFUNCTION(BlueprintCallable)
	void InitializeCurrentPersistentLevel();

	void LoadNextStreamingLevel(const TArray<FName>& StreamingLevelList, int32 Index);

private:

	UFUNCTION(BlueprintCallable)
	void YourCallbackFunction(int32 Linkage);

	UFUNCTION(BlueprintCallable)
	void OnAllStreamingLevelsLoaded();

};
