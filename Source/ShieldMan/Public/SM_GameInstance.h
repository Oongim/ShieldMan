// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "Engine/GameInstance.h"
#include "SM_GameInstance.generated.h"

/**
 * 
 */

UCLASS()
class SHIELDMAN_API USM_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USM_GameInstance();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stage1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stage2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stage3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PlayerNumber;
	
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
		void SetPlayerNumber(int num);

	UFUNCTION(BlueprintCallable)
		int GetPlayerNumber();
};