// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Npc/NpcWorkspace.h"

// Sets default values
ANpcWorkspace::ANpcWorkspace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANpcWorkspace::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANpcWorkspace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

