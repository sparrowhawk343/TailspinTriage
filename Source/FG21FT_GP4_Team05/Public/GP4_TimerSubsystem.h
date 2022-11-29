// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP4_GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GP4_TimerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimerRanOut);

UCLASS()
class FG21FT_GP4_TEAM05_API UGP4_TimerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()


public:

	UGP4_TimerSubsystem(){
		GameInstance = nullptr;
		// Timer = TimerStartValue;
	}

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FTimerRanOut OnTimerRanOut;
	
	UPROPERTY()
	UGP4_GameInstance* GameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Timer = 120.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimerStartValue = 120.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bUsingTimer = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bTimerRunOut = false;
	
	UFUNCTION(BlueprintNativeEvent)
	void TimerRanOut();

	UFUNCTION(BlueprintCallable)
	void ResetTimer();

	UFUNCTION(BlueprintCallable)
	void StartTimer();

	UFUNCTION(BlueprintCallable)
	void StopTimer();

	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// FTickableGameObject implementation Begin
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetWorld(); }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual bool IsAllowedToTick() const override { return true; }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return TStatId(); }
	// FTickableGameObject implementation End
};
