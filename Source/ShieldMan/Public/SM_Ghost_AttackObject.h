// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_Ghost_AttackObject.generated.h"

UCLASS()
class SHIELDMAN_API ASM_Ghost_AttackObject : public AActor
{
	GENERATED_BODY()
private:
	float DeathMaxCount;

	float LifeTime;

	FTimerHandle DeathTimerHandle;
	FTimerHandle LifeTimerHandle;

	bool bDead;
	bool bDefended;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ShootBody)
		UStaticMeshComponent* Bullet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		USphereComponent* Collision;

	TSubclassOf<class AShieldManCharacter> characterClass;

	class AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		UParticleSystemComponent* MoveEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		UParticleSystem* EndEffect;


public:
	// Sets default values for this actor's properties
	ASM_Ghost_AttackObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		virtual void SetTarget(AActor* actor);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Death();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OnHittedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
		void StartNiagaraEffect();
};
