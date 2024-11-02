
#include "Core/Actor/Door.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"


ADoor::ADoor()
{
	// construct components
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	DoorMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshComp"));
	DoorMeshComp->SetupAttachment(RootComp);

	// door collision component
	DoorCollisionComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorCollisionComp"));
	DoorCollisionComp->SetupAttachment(DoorMeshComp);

	DoorCollisionComp->SetStaticMesh(DoorMeshComp->GetStaticMesh());

	DoorCollisionComp->SetVisibility(false);
	DoorCollisionComp->SetHiddenInGame(true);

	DoorCollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DoorCollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	DoorCollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	DoorCollisionComp->SetRelativeScale3D(FVector(1.25f, 1.0f, 1.0f));

	// attributes
	IsDoorOpen = false;
	DoorMovementDirection = FVector(0.0f, 0.0f, 0.0f);
	DoorMovementRotation = FRotator(0.0f, 0.0f, 0.0f);
	CheckCollision = true;
	OpeningSpeed = 1.0f;

	SetInteractableState(true);

	// setup timeline
	DoorMovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorMovementTimeline"));
}


void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (CheckCollision)
	{
		DoorCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnDoorCollisionOverlapBegin);
		DoorCollisionComp->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnDoorCollisionOverlapEnd);
	}
	else
	{
		DoorCollisionComp->DestroyComponent();
	}

	if (DoorMovementCurve)
	{
		FOnTimelineFloat DoorMovementTimelineProgress;
		DoorMovementTimelineProgress.BindUFunction(this, FName("HandleDoorMovementTimelineProgress"));
		DoorMovementTimeline->AddInterpFloat(DoorMovementCurve, DoorMovementTimelineProgress);

		FOnTimelineEvent DoorMovementTimelineFinished;
		DoorMovementTimelineFinished.BindUFunction(this, FName("HandleDoorMovementTimelineFinished"));
		DoorMovementTimeline->SetTimelineFinishedFunc(DoorMovementTimelineFinished);

		DoorMovementTimeline->SetLooping(false);
		DoorMovementTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		DoorMovementTimeline->SetPlayRate(OpeningSpeed);

	}
}


void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::StartInteraction_Implementation(UPrimitiveComponent* HitComponent, AActor* Interactor)
{
	SetInteractableState(false);
	DoorMovementTimeline->Stop();
	DoorMovementTimeline->SetPlayRate(OpeningSpeed);

	// check if door is locked
	if (IsDoorLocked)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorLockedSound, GetActorLocation());
	}
	else
	{
		if (IsDoorMovementBlocked())
		{
			StopDoorMovement();
		}

		// move the door
		if (IsDoorOpen)
		{
			IsDoorOpen = false;
			DoorMovementTimeline->Reverse();
		}
		else
		{
			IsDoorOpen = true;
			DoorMovementTimeline->Play();
			if (DoorMovementTimeline->GetPlaybackPosition() == 0.0f)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpeningSound, GetActorLocation());
			}

		}

		// allow interaction after some time
		FTimerHandle AllowInteractionTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(AllowInteractionTimerHandle, [this]()
			{
				DoorMeshComp->ComponentTags.AddUnique(FName("Interactable"));
			}, 0.5f, false);

	}

	OnInteraction.Broadcast();
}

void ADoor::StopInteraction_Implementation(UPrimitiveComponent* HitComponent, AActor* Interactor)
{
}

void ADoor::HandleDoorMovementTimelineProgress(float Value)
{
	if (IsDoorMovementBlocked())
	{
		StopDoorMovement();
	}
	else
	{
		// lerp door position according to movement direction config
		if (!DoorMovementDirection.IsZero())
		{
			const FVector DestLocation = FMath::Lerp(
				FVector(0.0f, 0.0f, 0.0f),
				DoorMovementDirection,
				Value);
			DoorMeshComp->SetRelativeLocation(DestLocation);

			if (IsDebugEnabled)
			{
				UE_LOG(LogTemp, Log, TEXT("DoorDebug :: Movement Location Value = %f, %f, %f"), DestLocation.X, DestLocation.Y, DestLocation.Z);
			}
		}

		// lerp door rotation according to movement rotation config
		if (!DoorMovementRotation.IsZero())
		{
			const FRotator CurrentDoorRotation = DoorMeshComp->GetRelativeRotation();
			const FRotator NewRotation = FMath::Lerp(FRotator(0.0f, 0.0f, 0.0f), DoorMovementRotation, Value);
			DoorMeshComp->SetRelativeRotation(NewRotation);

			if (IsDebugEnabled)
			{
				UE_LOG(LogTemp, Log, TEXT("DoorDebug :: Value = %f :: Movement Rotation from (%f, %f, %f) to (%f, %f, %f) = (%f, %f, %f)"), Value,
					CurrentDoorRotation.Pitch, CurrentDoorRotation.Yaw, CurrentDoorRotation.Roll,
					DoorMovementRotation.Pitch, DoorMovementRotation.Yaw, DoorMovementRotation.Roll,
					NewRotation.Pitch, NewRotation.Yaw, NewRotation.Roll);
			}
		}
	}
}

void ADoor::HandleDoorMovementTimelineFinished()
{
	if (DoorMovementTimeline->GetPlaybackPosition() == 0.0f)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorClosingSound, GetActorLocation());
	}
}

void ADoor::SetInteractableState(bool State)
{
	if (State)
	{
		DoorMeshComp->ComponentTags.AddUnique(FName("Interactable"));
	}
	else
	{
		DoorMeshComp->ComponentTags.Remove(FName("Interactable"));
	}
}

void ADoor::OnDoorCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoorMovementTimeline->IsPlaying() || DoorMovementTimeline->IsReversing())
	{
		StopDoorMovement();
	}
	
}

void ADoor::OnDoorCollisionOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ResumeDoorMovement();
}

bool ADoor::IsDoorInEndPosition()
{
	return DoorMovementTimeline->GetPlaybackPosition() == 0.0f || DoorMovementTimeline->GetPlaybackPosition() == 1.0f;
}

bool ADoor::IsDoorMovementBlocked()
{
	if (IsDoorInEndPosition())
	{
		return false;
	}
	else
	{
		TArray<AActor*> OverlappingActors;
		DoorCollisionComp->GetOverlappingActors(OverlappingActors);

		return OverlappingActors.Num() > 0;
	}
}

void ADoor::ResumeDoorMovement()
{
	if (IsDoorOpen)
	{
		DoorMovementTimeline->Play();
	}
	else
	{
		DoorMovementTimeline->Reverse();
	}
}

void ADoor::StopDoorMovement()
{
	if (DoorCollisionSpawnedSound == nullptr || !DoorCollisionSpawnedSound->IsPlaying())
	{
		DoorCollisionSpawnedSound = UGameplayStatics::SpawnSoundAtLocation(this, DoorCollisionSound, GetActorLocation());
	}
	
	DoorMovementTimeline->Stop();
}

