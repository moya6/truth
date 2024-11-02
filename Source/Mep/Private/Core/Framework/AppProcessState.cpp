// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Framework/AppProcessState.h"

#include "GameFramework/GameUserSettings.h"

UAppProcessState::UAppProcessState()
{
	Resolution = TEXT("1920x1080");
	OverallQuality = 2;
	RootConfig = Cast<URootConfiguration>(StaticLoadObject(URootConfiguration::StaticClass(), nullptr,
		TEXT("/Game/Config/RootConfig_Default")));
}

URootConfiguration* UAppProcessState::GetRootConfiguration()
{
	return RootConfig;
}

FString UAppProcessState::GetResolution()
{
	return Resolution;
}

void UAppProcessState::SetResolution(FString NewResolution)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		FString WidthString, HeightString;
		int OutWidth, OutHeight;

		if (NewResolution.Split(TEXT("x"), &WidthString, &HeightString))
		{
			OutWidth = FCString::Atoi(*WidthString);
			OutHeight = FCString::Atoi(*HeightString);

			UserSettings->SetScreenResolution(FIntPoint(OutWidth, OutHeight));
			UserSettings->ApplySettings(false);
			Resolution = NewResolution;
		}
		else
		{
			// Handle the case where the string does not contain 'x' or is invalid
			UE_LOG(LogTemp, Warning, TEXT("Invalid resolution format: %s"), *NewResolution);
		}
	}
}

bool UAppProcessState::GetVsync()
{
	return Vsync;
}

void UAppProcessState::SetVsync(bool NewVsync)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetVSyncEnabled(NewVsync);
		UserSettings->ApplySettings(false);
		Vsync = NewVsync;
	}
}

bool UAppProcessState::GetAntiAliasing()
{
	return AntiAliasing;
}

void UAppProcessState::SetAntiAliasing(bool NewAntiAliasing)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		if (NewAntiAliasing)
		{
			UserSettings->SetAntiAliasingQuality(2);
		}
		else
		{
			UserSettings->SetAntiAliasingQuality(0);
		}
		
		UserSettings->ApplySettings(false);
		AntiAliasing = NewAntiAliasing;
	}
}

int UAppProcessState::GetOverallQuality()
{
	return OverallQuality;
}

void UAppProcessState::SetOverallQuality(int NewOverallQuality)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetOverallScalabilityLevel(NewOverallQuality);

		UserSettings->ApplySettings(false);
		OverallQuality = NewOverallQuality;
	}
}



bool UAppProcessState::GetAppProcessInitialized()
{
	return AppProcessInitialized;
}

void UAppProcessState::SetAppProcessInitialized(bool NewAppProcessInitialized)
{
	AppProcessInitialized = NewAppProcessInitialized;
}

