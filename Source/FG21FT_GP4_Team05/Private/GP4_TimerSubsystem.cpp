// Fill out your copyright notice in the Description page of Project Settings.

#include "GP4_TimerSubsystem.h"
#include "Kismet/GameplayStatics.h"


void UGP4_TimerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GameInstance = Cast<UGP4_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	OnTimerRanOut.AddDynamic(this, &UGP4_TimerSubsystem::TimerRanOut);
}

void UGP4_TimerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	OnTimerRanOut.RemoveDynamic(this, &UGP4_TimerSubsystem::TimerRanOut);
}

void UGP4_TimerSubsystem::Tick(float DeltaTime)
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	if (bUsingTimer)
	{
		Timer = Timer - 1 * DeltaTime;

		if (Timer <= 0.0f)
		{
			Timer = 0.0f;
			if (OnTimerRanOut.IsBound())
			{
				OnTimerRanOut.Broadcast();
				// bTimerRunOut = true;
				// ResetTimer();
				StopTimer();
			}
		}

		
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(
		// 		-1,
		// 		1.0f,
		// 		FColor::Yellow,
		// 		FString::Printf(TEXT("Timer: %f, %d"), Timer, GetWorld() ? GetWorld()->IsGameWorld() : -1)
		// 	);
		// }
	}
}

void UGP4_TimerSubsystem::TimerRanOut_Implementation()
{
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(
	// 		-1,
	// 		5.0f,
	// 		FColor::Yellow,
	// 		FString::Printf(TEXT("Timer ran out!"))
	// 	);
	// }
}

//Delete this function if start timer and stop timer functions work.
void UGP4_TimerSubsystem::ResetTimer()
{
	Timer = TimerStartValue;
	bUsingTimer = false;
	bTimerRunOut = false;
}

void UGP4_TimerSubsystem::StartTimer()
{
	Timer = TimerStartValue;
	bUsingTimer = true;
	bTimerRunOut = false;
}

void UGP4_TimerSubsystem::StopTimer()
{
	bUsingTimer = false;
	bTimerRunOut = true;
}
