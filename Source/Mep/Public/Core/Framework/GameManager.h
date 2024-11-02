// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Core/Manager/PersistentLevelManager.h"

#include "GameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameManagerInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameInitialized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMilestoneUpdated, FName, MilestoneName);

UCLASS(minimalapi)
class AGameManager : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Initialization")
	FOnGameManagerInitialized OnGameManagerInitialized;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Initialization")
	FOnGameInitialized OnGameInitialized;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Game")
	FOnMilestoneUpdated OnMilestoneUpdated;

protected:

	APersistentLevelManager* PersistentLevelManager;

	bool GameManagerInitialized = false;

	/* Game State */

	UPROPERTY(EditAnywhere, Category = "Game State")
	TArray<FName> CompletedMilestones;

public:

	AGameManager();

	APersistentLevelManager* GetPersistentLevelManager();

	bool IsGameManagerInitialized();

	/* Game State */

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMilestoneCompleted(FName Milestone);

	UFUNCTION(BlueprintCallable)
	void SetCompletedMilestone(FName NewCompletedMilestone);

protected:

	virtual void BeginPlay() override;

	void InitializeGameManager();

	UFUNCTION(BlueprintNativeEvent, Category = "Initialization")
	void AfterGameManagerInitialized();
	virtual void AfterGameManagerInitialized_Implementation();

private:

	void InitializeAppProcessSettings();
};





