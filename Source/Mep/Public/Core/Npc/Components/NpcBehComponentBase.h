// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Data/Enum/NpcEnums.h"

#include "NpcBehComponentBase.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEP_API UNpcBehComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	
	ENpcBehState CurrentBehState;

public:	
	// Sets default values for this component's properties
	UNpcBehComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetBehState(ENpcBehState NewState);
};
