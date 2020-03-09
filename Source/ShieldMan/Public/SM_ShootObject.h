// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_ShootObject.generated.h"

UCLASS()
class SHIELDMAN_API ASM_ShootObject : public AActor
{
	GENERATED_BODY()
private:
	class AShieldManCharacter* Player;

	class ASM_ShootObjectBullet* SpawnPawn;

	float sleepTime;
	float maxSleepTime;
	bool bFire;
	
	float reloadMaxTime;
	float reloadTime;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category=ShootBody)
	UStaticMeshComponent* ShootBody;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf< class ASM_ShootObjectBullet> SpawnBulletClass;
public:	
	// Sets default values for this actor's properties
	ASM_ShootObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnBullet(FVector Loc, FRotator Rot);

	UFUNCTION()
	void StartAttack();

	UFUNCTION()
	void StopAttack();

};
