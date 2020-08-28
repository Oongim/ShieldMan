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
	float DeathMaxCount;

	float LifeTime;

	FTimerHandle DeathTimerHandle;
	FTimerHandle LifeTimerHandle;

	bool bDead;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = ShootBody)
	UStaticMeshComponent* Bullet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = Collision)
	USphereComponent* Collision;

	TSubclassOf<class AShieldManCharacter> characterClass;

public:	
	// Sets default values for this actor's properties
	ASM_ShootObjectBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void AddFroce(float power);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void ServerOverlapBegin(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
		virtual void ServerStartNiagaraEffect();

	UFUNCTION()
	void Death();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OnHittedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void StartNiagaraEffect();
};
