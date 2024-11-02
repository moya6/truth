#include "Data/DataAsset/GeneralGameplayConfig.h"

UGeneralGameplayConfig::UGeneralGameplayConfig()
{
    PlayerMovementSpeedBase = 250.0f;
    PlayerFovBase = 75.0f;

    InteractionTraceRange = 150.0f;
    InteractionTraceRadius = 10.0f;
    
    PlayerFovZoomed = 50.0f;
    PlayerMovementSpeedZoomedBase = 125.0f;
    PlayerZoomSpeed = 0.5f;

    NpcSightConeShapeDefault = FVector(8.0f, 8.0f, 10.0f);
}