// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/SceneComponent.h"

#include "Data/Enum/GeneralEnums.h"

#include "IDriveableVehicle.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UIDriveableVehicle : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MEP_API IIDriveableVehicle
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void Accelerate(float Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void Brake(float Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void Steer(float Value);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void Look(float XValue, float YValue);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void Handbrake(EBinaryStateChange action);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void Engine(EBinaryStateChange action);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void OpenDoor();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Control)
	void CloseDoor();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Other)
	USceneComponent* GetDriverLocation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Other)
	FTransform GetExitLocation();
};
