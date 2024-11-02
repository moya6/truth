// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Framework/CoreBlueprintFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

/* Core Game Framework */

UAppProcessState* UCoreBlueprintFunctionLibrary::GetAppProcessState(UObject* WorldContextObject)
{
    return (Cast<UAppProcessState>(UGameplayStatics::GetGameInstance(WorldContextObject)));
}

AGameManager* UCoreBlueprintFunctionLibrary::GetGameManager(UObject* WorldContextObject)
{
    return (Cast<AGameManager>(UGameplayStatics::GetGameMode(WorldContextObject)));
}

AGameWidgetManager* UCoreBlueprintFunctionLibrary::GetGameWidgetManager(UObject* WorldContextObject)
{
    return (Cast<AGameWidgetManager>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)->GetHUD()));
}

APlayerCharacterController* UCoreBlueprintFunctionLibrary::GetPlayerCharacterController(UObject* WorldContextObject)
{
    return (Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)));
}

APlayerCharacter* UCoreBlueprintFunctionLibrary::GetPlayerCharacterBase(UObject* WorldContextObject)
{
    return Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(WorldContextObject, 0));
}

/* Configuration */

URootConfiguration* UCoreBlueprintFunctionLibrary::GetRootConfig(UObject* WorldContextObject)
{
    return (Cast<UAppProcessState>(UGameplayStatics::GetGameInstance(WorldContextObject)))->GetRootConfiguration();
}

UPersistentLevelManagerConfig* UCoreBlueprintFunctionLibrary::GetPersistentLevelManagerConfig(UObject* WorldContextObject)
{
    return GetRootConfig(WorldContextObject)->PersistentLevelManagerConfig;
}

UPersistentLevelConfig* UCoreBlueprintFunctionLibrary::GetCurrentPersistentLevelConfig(UObject* WorldContextObject)
{
    return GetPersistentLevelManager(WorldContextObject)->GetPersistentLevelConfig(GetPersistentLevelManager(WorldContextObject)->GetCurrentPersistentLevel());
}

UGeneralGameplayConfig* UCoreBlueprintFunctionLibrary::GetGeneralGameplayConfig(UObject* WorldContextObject)
{
    return GetRootConfig(WorldContextObject)->GetGeneralGameplayConfig();
}

/* Manager */

APersistentLevelManager* UCoreBlueprintFunctionLibrary::GetPersistentLevelManager(UObject* WorldContextObject)
{
    return (Cast<AGameManager>(UGameplayStatics::GetGameMode(WorldContextObject)))->GetPersistentLevelManager();
}

/* Game State */

bool UCoreBlueprintFunctionLibrary::IsGameManagerInitialized(UObject* WorldContextObject)
{
    return (Cast<AGameManager>(UGameplayStatics::GetGameMode(WorldContextObject)))->IsGameManagerInitialized();
}

EPersistentLevel UCoreBlueprintFunctionLibrary::GetCurrentPersistentLevel(UObject* WorldContextObject)
{
    return GetPersistentLevelManager(WorldContextObject)->GetCurrentPersistentLevel();
}


