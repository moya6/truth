// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/Player/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ChildActorComponent.h"

#include "Core/Framework/CoreBlueprintFunctionLibrary.h"
#include "Core/Interface/IInteractable.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

/* Constructor */

APlayerCharacter::APlayerCharacter()
{
	// create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// create a Physics Handle Component
	GrabberComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("CppGrabber"));
	GrabberComponent->bSoftAngularConstraint = false;
	GrabberComponent->bSoftLinearConstraint = false;
	GrabberComponent->InterpolationSpeed = 500;

	// create timelines
	BoardDriverSeatTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BoardDriverSeatTimeline"));
	UnboardDriverSeatTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("UnboardDriverSeatTimeline"));
	MoveToVehicleExitTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveToVehicleExitTimeline"));

	// its more convenient to set dependencies via blueprint but I leave that as an example
	//static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Config/Curves/BoardDriverSeatCurve"));
	//if (Curve.Succeeded())
	//{
	//	BoardDriverSeatCurve = Curve.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve2(TEXT("/Game/Config/Curves/MoveToVehicleExitCurve"));
	//if (Curve2.Succeeded())
	//{
	//	MoveToVehicleExitCurve = Curve.Object;
	//}

}

/* Begin Play */

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Set Default Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (BoardDriverSeatCurve)
	{
		// Board Timeline
		
		FOnTimelineFloat BoardDriverSeatTimelineProgress;
		BoardDriverSeatTimelineProgress.BindUFunction(this, FName("HandleBoardDriverSeatTimelineProgress"));
		BoardDriverSeatTimeline->AddInterpFloat(BoardDriverSeatCurve, BoardDriverSeatTimelineProgress);

		FOnTimelineEvent BoardDriverSeatTimelineFinished;
		BoardDriverSeatTimelineFinished.BindUFunction(this, FName("HandleBoardDriverSeatTimelineFinished"));
		BoardDriverSeatTimeline->SetTimelineFinishedFunc(BoardDriverSeatTimelineFinished);

		BoardDriverSeatTimeline->SetLooping(false);
		BoardDriverSeatTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		BoardDriverSeatTimeline->SetPlayRate(1.0f);

		// Unboard Timeline

		FOnTimelineFloat UnboardDriverSeatTimelineProgress;
		UnboardDriverSeatTimelineProgress.BindUFunction(this, FName("HandleUnboardDriverSeatTimelineProgress"));
		UnboardDriverSeatTimeline->AddInterpFloat(BoardDriverSeatCurve, UnboardDriverSeatTimelineProgress);

		FOnTimelineEvent UnboardDriverSeatTimelineFinished;
		UnboardDriverSeatTimelineFinished.BindUFunction(this, FName("HandleUnboardDriverSeatTimelineFinished"));
		UnboardDriverSeatTimeline->SetTimelineFinishedFunc(UnboardDriverSeatTimelineFinished);

		UnboardDriverSeatTimeline->SetLooping(false);
		UnboardDriverSeatTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		UnboardDriverSeatTimeline->SetPlayRate(1.0f);
	}

	if (MoveToVehicleExitCurve)
	{
		// Move To Vehicle Exit Timeline

		FOnTimelineFloat MoveToVehicleExitTimelineProgress;
		MoveToVehicleExitTimelineProgress.BindUFunction(this, FName("HandleMoveToVehicleExitTimelineProgress"));
		MoveToVehicleExitTimeline->AddInterpFloat(MoveToVehicleExitCurve, MoveToVehicleExitTimelineProgress);

		FOnTimelineEvent MoveToVehicleExitTimelineFinished;
		MoveToVehicleExitTimelineFinished.BindUFunction(this, FName("HandleMoveToVehicleExitTimelineFinished"));
		MoveToVehicleExitTimeline->SetTimelineFinishedFunc(MoveToVehicleExitTimelineFinished);

		MoveToVehicleExitTimeline->SetLooping(false);
		MoveToVehicleExitTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		MoveToVehicleExitTimeline->SetPlayRate(1.0f);
	}

	// attributes
	
	InteractionTraceDebug = UCoreBlueprintFunctionLibrary::GetGeneralGameplayConfig(GetWorld())->InteractionTraceDebug;
	InteractionTraceRange = UCoreBlueprintFunctionLibrary::GetGeneralGameplayConfig(GetWorld())->InteractionTraceRange;
	InteractionTraceRadius = UCoreBlueprintFunctionLibrary::GetGeneralGameplayConfig(GetWorld())->InteractionTraceRadius;
	
	GetCharacterMovement()->MaxWalkSpeed = UCoreBlueprintFunctionLibrary::GetGeneralGameplayConfig(GetWorld())->PlayerMovementSpeedBase;


}

/* Tick */
void APlayerCharacter::Tick(float DeltaTime)
{
	TraceForInteraction();
}

/* Other */

void APlayerCharacter::TraceForInteraction()
{
	// Prepare trace 
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = Start + ((FirstPersonCameraComponent->GetComponentRotation().Vector()) * InteractionTraceRange);

	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult OutHit;

	// Perform trace
	const bool bHit = GetWorld()->SweepSingleByChannel(
		OutHit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(InteractionTraceRadius),
		TraceParams
	);

	InteractionActor = OutHit.GetActor();
	if (InteractionActor != nullptr)
	{
		UE_LOG(LogTemplateCharacter, Log, TEXT("'%s' Hit Actor: "), *InteractionActor->GetName());
	}

	// Handle trace result
	if (!PhotoCameraActivated && bHit && OutHit.GetActor()->GetClass()->ImplementsInterface(UIInteractable::StaticClass())
		&& OutHit.GetComponent()->ComponentHasTag(FName("Interactable")))
	{
		if (InteractionComponent != nullptr)
		{
			InteractionComponent->SetRenderCustomDepth(false);
		}

		InteractionActor = OutHit.GetActor();
		InteractionComponent = OutHit.GetComponent();
		InteractionComponent->SetRenderCustomDepth(true);

		if (InteractionTraceDebug)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 0.2f);
			//UE_LOG(LogTemplateCharacter, Log, TEXT("'%s' Traced Actor: "), InteractionActor->GetName());
		}
	}
	else
	{
		if (InteractionComponent != nullptr)
		{
			InteractionComponent->SetRenderCustomDepth(false);
		}
		InteractionActor = nullptr;
		InteractionComponent = nullptr;

		if (InteractionTraceDebug)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 0.2f);
		}
	}
}

/* Input */

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::StartInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopInteract);

		// Camera
		EnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCamera);
		EnhancedInputComponent->BindAction(CameraAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCamera);

		// Vehicle Look
		EnhancedInputComponent->BindAction(VehLookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::VehLook);

		// Vehicle Throttle
		EnhancedInputComponent->BindAction(VehThrottleAction, ETriggerEvent::Started, this, &APlayerCharacter::VehThrottle);
		EnhancedInputComponent->BindAction(VehThrottleAction, ETriggerEvent::Completed, this, &APlayerCharacter::VehThrottle);
		EnhancedInputComponent->BindAction(VehThrottleAction, ETriggerEvent::Ongoing, this, &APlayerCharacter::VehThrottle);

		// Vehicle Brake
		EnhancedInputComponent->BindAction(VehBrakeAction, ETriggerEvent::Started, this, &APlayerCharacter::VehBrake);
		EnhancedInputComponent->BindAction(VehBrakeAction, ETriggerEvent::Completed, this, &APlayerCharacter::VehBrake);
		EnhancedInputComponent->BindAction(VehBrakeAction, ETriggerEvent::Ongoing, this, &APlayerCharacter::VehBrake);

		// Vehicle Steer
		EnhancedInputComponent->BindAction(VehSteerAction, ETriggerEvent::Started, this, &APlayerCharacter::VehSteer);
		EnhancedInputComponent->BindAction(VehSteerAction, ETriggerEvent::Completed, this, &APlayerCharacter::VehSteer);
		EnhancedInputComponent->BindAction(VehSteerAction, ETriggerEvent::Triggered, this, &APlayerCharacter::VehSteer);

		// Vehicle Handbrake
		EnhancedInputComponent->BindAction(VehHandbrakeAction, ETriggerEvent::Started, this, &APlayerCharacter::VehHandbrakeStart);
		EnhancedInputComponent->BindAction(VehHandbrakeAction, ETriggerEvent::Completed, this, &APlayerCharacter::VehHandbrakeStop);

		// Vehicle Engine
		EnhancedInputComponent->BindAction(VehEngineAction, ETriggerEvent::Started, this, &APlayerCharacter::VehEngine);

		// Esc
		EnhancedInputComponent->BindAction(EscAction, ETriggerEvent::Started, this, &APlayerCharacter::HandleEsc);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component!"), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::VehLook(const FInputActionValue& Value)
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		const float InputY = Value.Get<FVector2D>().Y * (-1.0f);
		const float InputX = Value.Get<FVector2D>().X;

		float DeltaY = InputY  + GetFirstPersonCameraComponent()->GetRelativeRotation().Pitch;
		if (DeltaY > 35.0f || DeltaY < -45.0f)
		{
			DeltaY = GetFirstPersonCameraComponent()->GetRelativeRotation().Pitch;
		}

		float DeltaZ = InputX + GetFirstPersonCameraComponent()->GetRelativeRotation().Yaw;
		if (DeltaZ > 150.0f || DeltaZ < -150.0f)
		{
			DeltaZ = GetFirstPersonCameraComponent()->GetRelativeRotation().Yaw;
		}

		GetFirstPersonCameraComponent()->SetRelativeRotation(FRotator(DeltaY, DeltaZ, 0.0f));
	}
}

void APlayerCharacter::VehThrottle(const FInputActionValue& Value)
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		BoardedVehicle->Execute_Accelerate(BoardedVehicle.GetObject(), Value.Get<float>());
	}
}

void APlayerCharacter::VehBrake(const FInputActionValue& Value)
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		BoardedVehicle->Execute_Brake(BoardedVehicle.GetObject(), Value.Get<float>());
	}
}

void APlayerCharacter::VehSteer(const FInputActionValue& Value)
{

	if (BoardedVehicle.GetObject() != nullptr)
	{
		BoardedVehicle->Execute_Steer(BoardedVehicle.GetObject(), Value.Get<float>());
	}
}

void APlayerCharacter::VehHandbrakeStart(const FInputActionValue& Value)
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		BoardedVehicle->Execute_Handbrake(BoardedVehicle.GetObject(), EBinaryStateChange::TurnOn);
	}
}

void APlayerCharacter::VehHandbrakeStop(const FInputActionValue& Value)
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		BoardedVehicle->Execute_Handbrake(BoardedVehicle.GetObject(), EBinaryStateChange::TurnOff);
	}
}

void APlayerCharacter::VehEngine(const FInputActionValue& Value)
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		BoardedVehicle->Execute_Engine(BoardedVehicle.GetObject(), EBinaryStateChange::Flip);
	}
}

void APlayerCharacter::StartInteract(const FInputActionValue& Value)
{
	if (PhotoCameraActivated)
	{
		HandleMakePhotoAction();
	}
	if (InteractionActor != nullptr)
	{
		if (InteractionActor->GetClass()->ImplementsInterface(UIInteractable::StaticClass()))
		{
			Cast<IIInteractable>(InteractionActor)->Execute_StartInteraction(InteractionActor, InteractionComponent, this);
		}
	}

}

void APlayerCharacter::StopInteract(const FInputActionValue& Value) 
{
	
}

void APlayerCharacter::StartCamera(const FInputActionValue& Value)
{
	HandleStartCameraAction();
}

void APlayerCharacter::StopCamera(const FInputActionValue& Value)
{
	HandleStopCameraAction();
}

void APlayerCharacter::HandleEsc(const FInputActionValue& Value)
{
	HandleEscapeAction();
}

void APlayerCharacter::HandleEscapeAction_Implementation() { }

void APlayerCharacter::HandleStartCameraAction_Implementation() { }

void APlayerCharacter::HandleStopCameraAction_Implementation() { }

void APlayerCharacter::HandleMakePhotoAction_Implementation() { }

/* Vehicle */

void APlayerCharacter::BoardVehicle_Implementation(const TScriptInterface<IIDriveableVehicle>& Vehicle)
{
	BoardedVehicle = Vehicle;
	VehicleDriverSeat = BoardedVehicle->Execute_GetDriverLocation(BoardedVehicle.GetObject());
	VehicleExitLocation = BoardedVehicle->Execute_GetExitLocation(BoardedVehicle.GetObject());

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetFirstPersonCameraComponent()->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = false;

	// clear current input mapping context
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(Controller)
		->GetLocalPlayer())
		->ClearAllMappings();

	MoveToVehicleExitTimeline->PlayFromStart();
}

void APlayerCharacter::UnboardVehicle_Implementation(FTransform ExitLocation)
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		VehicleExitLocation = ExitLocation;

		// clear current input mapping context
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(Controller)
			->GetLocalPlayer())
			->ClearAllMappings();

		this->K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative);
		UCoreBlueprintFunctionLibrary::GetPlayerCharacterController(GetWorld())->SetControlRotation(ExitLocation.Rotator());
		UnboardDriverSeatTimeline->PlayFromStart();
	}
}

bool APlayerCharacter::IsInVehicle_Implementation()
{
	if (BoardedVehicle.GetObject() != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* Move to vehicle exit timeline */

void APlayerCharacter::HandleMoveToVehicleExitTimelineProgress(float Value)
{
	//UE_LOG(LogTemplateCharacter, Log, TEXT("HandleMoveToVehicleExitTimelineProgress = %f"), Value);
	// lerp actor location
	const FVector DestLocation = FMath::Lerp(GetActorLocation(), VehicleExitLocation.GetLocation(), Value);
	GetCapsuleComponent()->SetWorldLocation(DestLocation);

	// lerp actor rotation
	const FQuat DestRotation = FQuat::SlerpFullPath(FQuat(GetActorRotation()),
		FQuat(VehicleExitLocation.GetRotation()), Value);
	GetCapsuleComponent()->SetRelativeRotation(DestRotation);

	// reset camera rotation after it is unattached from control rotation
	const FQuat DestCameraRotation = FQuat::Slerp(FQuat(GetFirstPersonCameraComponent()->GetRelativeRotation()),
		FQuat(FRotator(0.0f, 0.0f, 0.0f)), Value);
	GetFirstPersonCameraComponent()->SetRelativeRotation(DestCameraRotation);
}

void APlayerCharacter::HandleMoveToVehicleExitTimelineFinished()
{
	OnPlayerMovedToVehicleExitLocation.Broadcast();
	BoardDriverSeatTimeline->PlayFromStart();
}

/* Board to driver seat timeline */

void APlayerCharacter::HandleBoardDriverSeatTimelineProgress(float Value)
{
	//UE_LOG(LogTemplateCharacter, Log, TEXT("HandleBoardDriverSeatTimelineProgress = %f"), Value);

	// lerp actor location
	const FVector DestLocation = FMath::Lerp(GetActorLocation(), VehicleDriverSeat->GetComponentLocation(), Value);
	GetCapsuleComponent()->SetWorldLocation(DestLocation);
}

void APlayerCharacter::HandleBoardDriverSeatTimelineFinished()
{
	// attach driver to seat after moved in place
	K2_AttachToComponent(VehicleDriverSeat, FName("None"), EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepRelative, false);

	// setup vehicle controls
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(Controller)
		->GetLocalPlayer())
		->AddMappingContext(VehicleMappingContext, 0);

}

/* Unboard vehicle timeline */

void APlayerCharacter::HandleUnboardDriverSeatTimelineProgress(float Value)
{
	// lerp actor location
	const FVector DestLocation = FMath::Lerp(GetActorLocation(), VehicleExitLocation.GetLocation(), Value);
	GetCapsuleComponent()->SetWorldLocation(DestLocation);
	
	// lerp actor rotation
	const FQuat DestRotation = FQuat::Slerp(FQuat(GetActorRotation()),
		FQuat(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)), Value);
	GetCapsuleComponent()->SetRelativeRotation(DestRotation);

	// reset camera rotation after it is unattached from control rotation
	const FQuat DestCameraRotation = FQuat::Slerp(FQuat(GetFirstPersonCameraComponent()->GetRelativeRotation()),
		FQuat(FRotator(GetControlRotation().Pitch, 0.0f, 0.0f)), Value);
	GetFirstPersonCameraComponent()->SetRelativeRotation(DestCameraRotation);
}

void APlayerCharacter::HandleUnboardDriverSeatTimelineFinished()
{
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(Controller)
		->GetLocalPlayer())
		->AddMappingContext(DefaultMappingContext, 0);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetFirstPersonCameraComponent()->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	BoardedVehicle = nullptr;
}