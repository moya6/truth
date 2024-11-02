// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Data/DataAsset/RootConfiguration.h"

#include "AppProcessState.generated.h"

/**
 * 
 */
UCLASS()
class MEP_API UAppProcessState : public UGameInstance
{
	GENERATED_BODY()

protected:

	/* Config */

	UPROPERTY(EditAnywhere, Category = "Config")
	URootConfiguration* RootConfig;

	/* Settings */

	UPROPERTY(EditAnywhere, Category = "Settings")
	FString Resolution;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool Vsync;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool AntiAliasing;

	UPROPERTY(EditAnywhere, Category = "Settings",
		meta = (
			ClampMin = "1", ClampMax = "4", UIMin = "1", UIMax = "4"))
	int OverallQuality;

private:

	/* App State */

	bool AppProcessInitialized;

public:

	UAppProcessState();

	/* Config */

	UFUNCTION(BlueprintCallable, BlueprintPure)
	URootConfiguration* GetRootConfiguration();

	/* Settings */

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetResolution();

	UFUNCTION(BlueprintCallable)
	void SetResolution(FString NewResolution);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetVsync();

	UFUNCTION(BlueprintCallable)
	void SetVsync(bool NewVsync);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetAntiAliasing();

	UFUNCTION(BlueprintCallable)
	void SetAntiAliasing(bool NewAntiAliasing);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetOverallQuality();

	UFUNCTION(BlueprintCallable)
	void SetOverallQuality(int NewOverallQuality);

	/* App State */

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetAppProcessInitialized();

	void SetAppProcessInitialized(bool NewAppProcessInitialized);
	
};
