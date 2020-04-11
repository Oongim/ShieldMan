// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "SM_Converyor.generated.h"

UCLASS()
class SHIELDMAN_API ASM_Converyor : public AActor
{
	GENERATED_BODY()

private:

	bool bisOverlap;


public:
	// Sets default values for this actor's properties
	ASM_Converyor();

	UPROPERTY(BlueprintReadWrite, Category = Collision)
		UBoxComponent* On_Collision;

	UPROPERTY(EditAnywhere, Category = "Value")
		float Speed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = VS)
		void SetCollisionFromBP(UBoxComponent* Col);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
