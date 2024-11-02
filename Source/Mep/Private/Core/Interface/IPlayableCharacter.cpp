// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Interface/IPlayableCharacter.h"

// Add default functionality here for any IIPlayableCharacter functions that are not pure virtual.
void IIPlayableCharacter::BoardVehicle_Implementation(const TScriptInterface<IIDriveableVehicle>& Vehicle)
{
}

void IIPlayableCharacter::UnboardVehicle_Implementation(FTransform ExitLocation)
{
}

bool IIPlayableCharacter::IsInVehicle_Implementation()
{
	return false;
}
