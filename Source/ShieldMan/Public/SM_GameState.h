// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/GameStateBase.h"
#include "SM_GameState.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API ASM_GameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
		FRotator ControllerRot;

	UPROPERTY(Replicated)
		FVector RightHandPos;

	UPROPERTY(Replicated)
		FVector LeftHandPos;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		float Stage1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		float Stage2;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		float Stage3;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
		float EntireTime;

	UFUNCTION(BlueprintCallable)
		void SetTimeStage1(float time);

	UFUNCTION(BlueprintCallable)
		void SetTimeStage2(float time);

	UFUNCTION(BlueprintCallable)
		void SetTimeStage3(float time);

	UFUNCTION(BlueprintCallable)
		float GetTimeStage1();

	UFUNCTION(BlueprintCallable)
		float GetTimeStage2();

	UFUNCTION(BlueprintCallable)
		float GetTimeStage3();

	UFUNCTION(BlueprintCallable)
		void AddEntireTime(float time);

	UFUNCTION(BlueprintCallable)
		float GetEntireTime();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
