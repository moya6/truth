// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Vehicle/VehicleBase.h"

float AVehicleBase::GetCurrentEngineRPM() const
{
    if (UChaosWheeledVehicleMovementComponent* ThisVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        return ThisVehicleMovementComponent->GetEngineRotationSpeed();
    }
    return 0.0f;
}

float AVehicleBase::GetMaxEngineRPM() const
{
    if (UChaosWheeledVehicleMovementComponent* ThisVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent()))
    {
        return ThisVehicleMovementComponent->GetEngineMaxRotationSpeed();
    }
    return 0.0f;
}

void AVehicleBase::SetMaxEngineTorque(float Value) const
{
    Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovementComponent())->SetMaxEngineTorque(Value);
}
