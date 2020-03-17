// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_SpawnToPointArea.generated.h"

UCLASS()
class SHIELDMAN_API ASM_SpawnToPointArea : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
		class UBoxComponent* AreaBox;
	
	UPROPERTY(EditAnywhere, meta = (makeEditWidget = "true"))
		FVector SpawnPoint;

public:	
	// Sets default values for this actor's properties
	ASM_SpawnToPointArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
