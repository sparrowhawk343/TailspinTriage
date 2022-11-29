#include "MultipleTargetCamera.h"

#include "GP4_GameInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"


AMultipleTargetCamera::AMultipleTargetCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Camera = GetCameraComponent();
	Camera->SetupAttachment(Root);
}

void AMultipleTargetCamera::BeginPlay()
{
	Super::BeginPlay();

	// APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	// Controller->SetViewTarget(this);
	// GetTargets(PlayerList);
	GameInstance = UGP4_GameInstance::Get(this);
	GameInstance->PlayerList.Empty();
	
}

void AMultipleTargetCamera::Tick(float DeltaTime)
{
	int NumberOfHelicopters = 0;
	FVector LocationSum = FVector::ZeroVector;

	for (APawn* Player : GameInstance->PlayerList)
	{
		if (Player == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					5.0f,
					FColor::Yellow,
					FString::Printf(TEXT("Player is null"))
					);
			}
		}
		if (Player && Player->IsActorInitialized())
		{
			LocationSum += Player->GetActorLocation();
			NumberOfHelicopters++;

			// if (GEngine)
			// {
			// 	GEngine->AddOnScreenDebugMessage(
			// 		-1,
			// 		5.0f,
			// 		FColor::Yellow,
			// 		FString::Printf(TEXT("Location: %s "), *Player->GetActorLocation().ToString())
			// 		);
			// }
		}
	}
	
	if (NumberOfHelicopters == 0)
	{
		return;
	}

	FVector TargetLocation = LocationSum / NumberOfHelicopters;

	TArray<FVector> PlayerLocations;
	
	for (APawn* Player : GameInstance->PlayerList)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		PlayerLocations.Add(PlayerLocation);
	}
	
	float LargestDistance = 0.0f;

	for (FVector PlayerLocationA : PlayerLocations)
	{
		for (FVector PlayerLocationB : PlayerLocations)
		{
			const float DistanceBetweenPlayers = FVector::Distance(PlayerLocationA, PlayerLocationB);
			if (DistanceBetweenPlayers >= LargestDistance)
			{
				LargestDistance = DistanceBetweenPlayers;
			}
		}
	}
	
	LargestDistance = FMath::Clamp(LargestDistance, MinPlayerDistance, MaxPlayerDistance);

	// Moving the camera
	FVector Location = GetActorLocation();
	Location = FMath::Lerp(Location, TargetLocation, FollowSpeed * DeltaTime);
	SetActorLocation(Location);

	// Zooming the camera
	float PercentageToZoom = UKismetMathLibrary::NormalizeToRange(LargestDistance, MinPlayerDistance, MaxPlayerDistance);
	float ZoomValue = FMath::Lerp(MinCameraZoom, MaxCameraZoom, PercentageToZoom);
	
	Location = Location + UKismetMathLibrary::NegateVector(GetActorForwardVector())  * ZoomValue * DeltaTime;
	SetActorLocation(Location);
	
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(
	// 		-1,
	// 		5.0f,
	// 		FColor::Yellow,
	// 		FString::Printf(TEXT("Location: %s "), *Location.ToString())
	// 	);
	// }
}
