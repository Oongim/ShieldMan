// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "SM_LAVA.generated.h"

UCLASS()
class SHIELDMAN_API ASM_LAVA : public AActor
{
	GENERATED_BODY()
	
private:


public:	
	// Sets default values for this actor's properties
	ASM_LAVA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly)
	bool downfall;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadWrite)
		UBoxComponent* downfallLAVA;

	UFUNCTION(BlueprintCallable, Category = VS)
		void SetCollisionFromBP(UBoxComponent* Col);

	class AShieldManCharacter* Player;

	UFUNCTION(BlueprintCallable, Category = VS)
	void Begin_downfallLAVA(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
		void DownPlane();

};
