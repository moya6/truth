// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "WheeledVehiclePawn.h"

#include "VehicleBase.generated.h"

/**
 * Exposes stuff which is not available via blueprints directly.
 */
UCLASS()
class MEP_API AVehicleBase : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
protected:

    UFUNCTION(BlueprintCallable, Category = "Engine")
    float GetCurrentEngineRPM() const;

    
    UFUNCTION(BlueprintCallable, Category = "Engine")
    float GetMaxEngineRPM() const;

    UFUNCTION(BlueprintCallable, Category = "Engine")
    void SetMaxEngineTorque(float Value) const;
};
