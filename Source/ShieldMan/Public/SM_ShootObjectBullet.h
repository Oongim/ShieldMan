// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_ShootObjectBullet.generated.h"

UCLASS()
class SHIELDMAN_API ASM_ShootObjectBullet : public AActor
{
	GENERATED_BODY()
private:
	int32 count;

	bool bAlive;

public:
	UPROPERTY(BlueprintReadOnly, Category = ShootBody)
	UStaticMeshComponent* Bullet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = Collision)
	USphereComponent* Collision;

public:	
	// Sets default values for this actor's properties
	ASM_ShootObjectBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsAlive();

	void Init();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
