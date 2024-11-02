// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/Framework/GameManager.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/GameUserSettings.h"
#include "UObject/ConstructorHelpers.h"

#include "Core/Player/PlayerCharacter.h"
#include "Core/Framework/CoreBlueprintFunctionLibrary.h"

AGameManager::AGameManager() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT(""));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AGameManager::BeginPlay()
{
	InitializeGameManager();
}

void AGameManager::AfterGameManagerInitialized_Implementation() {}

void AGameManager::InitializeGameManager()
{
	GEngine->bEnableOnScreenDebugMessages = UCoreBlueprintFunctionLibrary::GetRootConfig(GetWorld())->EnableEditorOnScreenMessages;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	PersistentLevelManager = GetWorld()->SpawnActor<APersistentLevelManager>(APersistentLevelManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	// Initialization completed
	GameManagerInitialized = true;
	OnGameManagerInitialized.Broadcast();
	AfterGameManagerInitialized();

	UE_LOG(LogTemp, Log, TEXT("Game Manager Initialization Completed"));

	InitializeAppProcessSettings();
}

APersistentLevelManager* AGameManager::GetPersistentLevelManager()
{
	return PersistentLevelManager;
}

bool AGameManager::IsGameManagerInitialized()
{
	return GameManagerInitialized;
}

bool AGameManager::IsMilestoneCompleted(FName Milestone)
{
	return CompletedMilestones.Contains(Milestone);
}

void AGameManager::SetCompletedMilestone(FName NewCompletedMilestone)
{
	if (!IsMilestoneCompleted(NewCompletedMilestone))
	{
		CompletedMilestones.AddUnique(NewCompletedMilestone);
		OnMilestoneUpdated.Broadcast(NewCompletedMilestone);
	}
}

void AGameManager::InitializeAppProcessSettings()
{
	if (!(UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->GetAppProcessInitialized()))
	{
		FString Resolution = UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->GetResolution();
		UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->SetResolution(Resolution);

		const bool AntiAliasing = UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->GetAntiAliasing();
		UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->SetAntiAliasing(AntiAliasing);

		const bool Vsync = UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->GetVsync();
		UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->SetVsync(Vsync);

		const int OverallQuality = UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->GetOverallQuality();
		UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->SetOverallQuality(OverallQuality);
		
		// additional hardcoded setup

		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
			UserSettings->ApplySettings(false);
		}

		GEngine->Exec(GetWorld(), TEXT("r.ScreenPercentage 100"));
		GEngine->Exec(GetWorld(), TEXT("sg.MotionBlurQuality 0")); 
		GEngine->Exec(GetWorld(), TEXT("r.MotionBlur.Amount 0.0")); 
		GEngine->Exec(GetWorld(), TEXT("r.BloomQuality 0"));

		UCoreBlueprintFunctionLibrary::GetAppProcessState(GetWorld())->SetAppProcessInitialized(true);
		UE_LOG(LogTemp, Log, TEXT("Game Manager :: Initialize App Process Settings Completed"));
	}
}
