// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_SetShieldLaser.generated.h"

UCLASS()
class SHIELDMAN_API ASM_SetShieldLaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASM_SetShieldLaser();
	
private:
	UFUNCTION()
		void OnCharacterOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = Lazer)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Lazer)
	UStaticMeshComponent* Lazer;

	UPROPERTY(EditInstanceOnly, Category = Lazer)
	TSubclassOf<class ASM_Shield> ShieldClass;
};
