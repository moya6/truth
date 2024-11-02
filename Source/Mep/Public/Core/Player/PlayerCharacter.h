// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "Core/Interface/IPlayableCharacter.h"

#include "PlayerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UPhysicsHandleComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerMovedToVehicleExitLocation);

UCLASS(config = Game)
class APlayerCharacter : public ACharacter, public IIPlayableCharacter
{
	GENERATED_BODY()

public:

	/* Dispatchers */

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Initialization")
	FOnPlayerMovedToVehicleExitLocation OnPlayerMovedToVehicleExitLocation;

	/* Components */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UPhysicsHandleComponent* GrabberComponent;

	/* Attributes - Mapping Context */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* VehicleMappingContext;

	/* Attributes - Input Action */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VehLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VehThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VehBrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VehSteerAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VehHandbrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VehEngineAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EscAction;

	/* Interaction */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	AActor* InteractionActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UPrimitiveComponent* InteractionComponent;

	/* Vehicle */

	// move to vehicle exit timeline
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MoveToVehicleExitCurve;

	UPROPERTY()
	UTimelineComponent* MoveToVehicleExitTimeline;

	UFUNCTION()
	void HandleMoveToVehicleExitTimelineProgress(float Value);

	UFUNCTION()
	void HandleMoveToVehicleExitTimelineFinished();

	// board to driver seat timeline
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* BoardDriverSeatCurve;

	UPROPERTY()
	UTimelineComponent* BoardDriverSeatTimeline;

	UFUNCTION()
	void HandleBoardDriverSeatTimelineProgress(float Value);

	UFUNCTION()
	void HandleBoardDriverSeatTimelineFinished();

	// unboard vehicle timeline
	UPROPERTY()
	UTimelineComponent* UnboardDriverSeatTimeline;

	UFUNCTION()
	void HandleUnboardDriverSeatTimelineProgress(float Value);

	UFUNCTION()
	void HandleUnboardDriverSeatTimelineFinished();

	/* Tools */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tools")
	bool PhotoCameraActivated;

	/* Attributes - Gameplay */

	UPROPERTY(EditAnywhere, Category = "Player Parameters")
	bool InteractionTraceDebug;

	UPROPERTY(EditAnywhere, Category = "Player Parameters")
	float InteractionTraceRange;

	UPROPERTY(EditAnywhere, Category = "Player Parameters")
	float InteractionTraceRadius;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IIDriveableVehicle> BoardedVehicle;

	USceneComponent* VehicleDriverSeat;

	FTransform VehicleExitLocation;

public:

	/* Constructor */

	APlayerCharacter();

	/* Called every frame */
	virtual void Tick(float DeltaTime) override;

	/* Getters and Setters */

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPhysicsHandleComponent* GetGrabberComponent() const { return GrabberComponent; }

protected:

	/* Overrides */

	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/* Input */
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void VehLook(const FInputActionValue& Value);

	void VehThrottle(const FInputActionValue& Value);

	void VehBrake(const FInputActionValue& Value);

	void VehSteer(const FInputActionValue& Value);

	void VehHandbrakeStart(const FInputActionValue& Value);

	void VehHandbrakeStop(const FInputActionValue& Value);

	void VehEngine(const FInputActionValue& Value);

	void StartInteract(const FInputActionValue& Value);

	void StopInteract(const FInputActionValue& Value);

	void StartCamera(const FInputActionValue& Value);

	void StopCamera(const FInputActionValue& Value);

	void HandleEsc(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void HandleEscapeAction();
	virtual void HandleEscapeAction_Implementation();

	/* Photo Camera */
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void HandleStartCameraAction();
	virtual void HandleStartCameraAction_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void HandleStopCameraAction();
	virtual void HandleStopCameraAction_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void HandleMakePhotoAction();
	virtual void HandleMakePhotoAction_Implementation();

	/* Other */

	void TraceForInteraction();

public:

	virtual void BoardVehicle_Implementation(const TScriptInterface<IIDriveableVehicle>& Vehicle) override;

	virtual void UnboardVehicle_Implementation(FTransform ExitLocation) override;

	virtual bool IsInVehicle_Implementation() override;
};
