// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UIInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MEP_API IIInteractable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Interaction)
	void StartInteraction(UPrimitiveComponent* HitComponent, AActor* Interactor);

	virtual void StartInteraction_Implementation(UPrimitiveComponent* HitComponent, AActor* Interactor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Interaction)
	void StopInteraction(UPrimitiveComponent* HitComponent, AActor* Interactor);

	virtual void StopInteraction_Implementation(UPrimitiveComponent* HitComponent, AActor* Interactor);

};
