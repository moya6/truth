// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Core/Framework/AppProcessState.h"
#include "Core/Framework/GameManager.h"
#include "Core/Framework/GameWidgetManager.h"
#include "Data/DataAsset/RootConfiguration.h"
#include "Core/Manager/PersistentLevelManager.h"
#include "Core/Player/PlayerCharacter.h"
#include "Core/Player/PlayerCharacterController.h"

#include "CoreBlueprintFunctionLibrary.generated.h"

UCLASS()
class MEP_API UCoreBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Core Game Framework */

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Manager, meta = (WorldContext = "WorldContextObject"))
	static UAppProcessState* GetAppProcessState(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Manager, meta = (WorldContext = "WorldContextObject"))
	static AGameManager* GetGameManager(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Manager, meta = (WorldContext = "WorldContextObject"))
	static AGameWidgetManager* GetGameWidgetManager(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Manager, meta = (WorldContext = "WorldContextObject"))
	static APlayerCharacterController* GetPlayerCharacterController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = State, meta = (WorldContext = "WorldContextObject"))
	static APlayerCharacter* GetPlayerCharacterBase(UObject* WorldContextObject);
	
	/* Configuration */

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Configuration, meta = (WorldContext = "WorldContextObject"))
	static URootConfiguration* GetRootConfig(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Configuration, meta = (WorldContext = "WorldContextObject"))
	static UPersistentLevelManagerConfig* GetPersistentLevelManagerConfig(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Configuration, meta = (WorldContext = "WorldContextObject"))
	static UPersistentLevelConfig* GetCurrentPersistentLevelConfig(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Configuration, meta = (WorldContext = "WorldContextObject"))
	static UGeneralGameplayConfig* GetGeneralGameplayConfig(UObject* WorldContextObject);
	
	/* Manager */

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Manager, meta = (WorldContext = "WorldContextObject"))
	static APersistentLevelManager* GetPersistentLevelManager(UObject* WorldContextObject);

	/* Game State */

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = State, meta = (WorldContext = "WorldContextObject"))
	static bool IsGameManagerInitialized(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = State, meta = (WorldContext = "WorldContextObject"))
	static EPersistentLevel GetCurrentPersistentLevel(UObject* WorldContextObject);
	
};
