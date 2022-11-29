#pragma once
#include "GP4_GameInstance.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "MultipleTargetCamera.generated.h"

// class UCameraComponent;


UCLASS()
class AMultipleTargetCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AMultipleTargetCamera();


	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void GetTargets(TArray<APawn*>& OutArray);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera Follow")
	float FollowSpeed = 10.0f;

	UPROPERTY()
	UGP4_GameInstance* GameInstance;

	// Change these back to EditAnywhere for designer tweaking
	UPROPERTY(EditAnywhere)
	float MinCameraZoom = 20000.0f;

	UPROPERTY(EditAnywhere)
	float MaxCameraZoom = 30000.0f;

	UPROPERTY(EditAnywhere)
	float MinPlayerDistance = 100.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxPlayerDistance = 1500.0f;

	
};