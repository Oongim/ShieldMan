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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Network)
	class UNetworkManager* networkManager;

	FTimerHandle HostTimer;

	int playercnt;
	
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



public:
	UFUNCTION(BlueprintCallable, Exec)
		void Host();

	UFUNCTION(BlueprintCallable, Exec)
		void Join(const  FString& Address);

	UFUNCTION(BlueprintCallable)
		UNetworkManager* GetNetworkManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString name;
};