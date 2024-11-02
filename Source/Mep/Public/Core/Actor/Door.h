// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/AudioComponent.h"

#include "Core/Interface/IInteractable.h"
#include "Sound/SoundBase.h"

#include "Door.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteraction);

UCLASS()
class MEP_API ADoor : public AActor, public IIInteractable
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Interaction")
	FOnInteraction OnInteraction;

	/* Components */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorCollisionComp;

	/* FX */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Audio Config")
	USoundBase* DoorOpeningSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Audio Config")
	USoundBase* DoorClosingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Audio Config")
	USoundBase* DoorLockedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Audio Config")
	USoundBase* DoorCollisionSound;

	/* Behavior Attributes */

	UPROPERTY(EditAnywhere, Category = "Door Behavior Config", meta = (AllowPrivateAccess = "true"))
	FVector DoorMovementDirection;

	UPROPERTY(EditAnywhere, Category = "Door Behavior Config", meta = (AllowPrivateAccess = "true"))
	FRotator DoorMovementRotation;

	UPROPERTY(EditAnywhere, Category = "Door Behavior Config")
	UCurveFloat* DoorMovementCurve;

	UPROPERTY(EditAnywhere, Category = "Door Behavior Config", meta = (AllowPrivateAccess = "true"))
	bool Bidirectional;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Behavior Config", meta = (AllowPrivateAccess = "true"))
	float OpeningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Behavior Config", meta = (AllowPrivateAccess = "true"))
	bool IsDoorLocked;

	UPROPERTY(EditAnywhere, Category = "Door Behavior Config", meta = (AllowPrivateAccess = "true"))
	bool CheckCollision;

	/* Movement Timeline */

	UPROPERTY()
	UTimelineComponent* DoorMovementTimeline;

	UFUNCTION()
	void HandleDoorMovementTimelineProgress(float Value);

	UFUNCTION()
	void HandleDoorMovementTimelineFinished();

	/* Dev */
	UPROPERTY(EditAnywhere, Category = "Door Dev Config", meta = (AllowPrivateAccess = "true"))
	bool IsDebugEnabled;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door State", meta = (AllowPrivateAccess = "true"))
	bool IsDoorOpen;

	UAudioComponent* DoorCollisionSpawnedSound;

public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartInteraction_Implementation(UPrimitiveComponent* HitComponent, AActor* Interactor) override;

	virtual void StopInteraction_Implementation(UPrimitiveComponent* HitComponent, AActor* Interactor) override;

private:

	void SetInteractableState(bool State);

	UFUNCTION()
	void OnDoorCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnDoorCollisionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsDoorInEndPosition();

	bool IsDoorMovementBlocked();

	void ResumeDoorMovement();

	void StopDoorMovement();
};
