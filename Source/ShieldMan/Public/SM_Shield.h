// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_Shield.generated.h"

UCLASS()
class SHIELDMAN_API ASM_Shield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASM_Shield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Shield;

	/*UPROPERTY(VisibleAnywhere, Category = Weapon)
		UStaticMeshComponent* Shield;*/
};
