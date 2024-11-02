// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Manager/PersistentLevelManager.h"

#include "Kismet/GameplayStatics.h"

#include "Core/Framework/CoreBlueprintFunctionLibrary.h"

// Sets default values
APersistentLevelManager::APersistentLevelManager()
{
	//
}

// Called when the game starts or when spawned
void APersistentLevelManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APersistentLevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APersistentLevelManager::OpenPersistentLevel(EPersistentLevel level)
{
	UGameplayStatics::OpenLevel(this, *(GetPersistentLevelConfig(level))->PersistentLevelName, true);
}

EPersistentLevel APersistentLevelManager::GetCurrentPersistentLevel()
{
	// get persistent level config map
	FString CurrentPersistentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	TMap<EPersistentLevel, UPersistentLevelConfig*> PersistentLevelConfigMap = UCoreBlueprintFunctionLibrary::GetPersistentLevelManagerConfig(GetWorld())->PersistentLevelConfigMap;

	// get map keyset
	TArray<EPersistentLevel> Keys;
	PersistentLevelConfigMap.GetKeys(Keys);

	// return the enum value associated with current level name string
	for (EPersistentLevel Key : Keys)
	{
		if (*(*(PersistentLevelConfigMap).Find(Key))->PersistentLevelName == CurrentPersistentLevelName)
		{
			return Key;
		}
	}

	return EPersistentLevel::None;
}

UPersistentLevelConfig* APersistentLevelManager::GetPersistentLevelConfig(EPersistentLevel level)
{
	return *(UCoreBlueprintFunctionLibrary::GetPersistentLevelManagerConfig(GetWorld()))->PersistentLevelConfigMap.Find(level);
}

void APersistentLevelManager::InitializeCurrentPersistentLevel() 
{
	TArray<FName> StreamingLevelList = UCoreBlueprintFunctionLibrary::GetCurrentPersistentLevelConfig(GetWorld())->BaseStreamingLevelList;

	// Start loading the first streaming level
	LoadNextStreamingLevel(StreamingLevelList, 0);
}

void APersistentLevelManager::LoadNextStreamingLevel(const TArray<FName>& StreamingLevelList, int32 Index)
{
	UE_LOG(LogTemp, Log, TEXT("PersistentLevelManagerDebug :: Loading Stream Level Init = %i"), Index);

	// Check if the index is valid
	if (StreamingLevelList.IsValidIndex(Index))
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "YourCallbackFunction";
		LatentInfo.UUID = Index + 1;
		LatentInfo.Linkage = Index;

		// Load the streaming level at the current index
		UE_LOG(LogTemp, Log, TEXT("PersistentLevelManagerDebug :: Loading Stream Level Start = %i"), Index);
		UGameplayStatics::LoadStreamLevel(GetWorld(), StreamingLevelList[Index], true, true, LatentInfo);
	}
	else
	{
		// All streaming levels have been loaded
		OnAllStreamingLevelsLoaded();
	}
}

void APersistentLevelManager::YourCallbackFunction(int32 Linkage)
{
	UE_LOG(LogTemp, Log, TEXT("PersistentLevelManagerDebug :: Loading Stream Level Completed = %i"), Linkage);
	// Handle completion of the load stream level operation
	int32 NextIndex = Linkage + 1;
	LoadNextStreamingLevel(UCoreBlueprintFunctionLibrary::GetCurrentPersistentLevelConfig(GetWorld())->BaseStreamingLevelList, NextIndex);
}

void APersistentLevelManager::OnAllStreamingLevelsLoaded()
{
	UE_LOG(LogTemp, Log, TEXT("PersistentLevelManagerDebug :: Loading Stream Level Operation End"));
	OnPersistentLevelInitialized.Broadcast();
}