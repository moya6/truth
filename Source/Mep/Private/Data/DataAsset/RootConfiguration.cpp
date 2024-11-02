// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAsset/RootConfiguration.h"

UGeneralGameplayConfig* URootConfiguration::GetGeneralGameplayConfig() const
{
#if WITH_EDITOR
    return GeneralGameplayConfigEditor;
#else
    return GeneralGameplayConfigProduction;
#endif
}
