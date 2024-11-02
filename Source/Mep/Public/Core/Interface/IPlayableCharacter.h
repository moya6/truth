// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Interface/IDriveableVehicle.h"

#include "IPlayableCharacter.generated.h"

UINTERFACE(BlueprintType)
class UIPlayableCharacter : public UInterface
{
	GENERATED_BODY()
};

class MEP_API IIPlayableCharacter
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Vehicle)
	void BoardVehicle(const TScriptInterface<IIDriveableVehicle>& Vehicle);

	virtual void BoardVehicle_Implementation(const TScriptInterface<IIDriveableVehicle>& Vehicle);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Vehicle)
	void UnboardVehicle(FTransform ExitLocation);

	virtual void UnboardVehicle_Implementation(FTransform ExitLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Vehicle)
	bool IsInVehicle();

	virtual bool IsInVehicle_Implementation();
};
