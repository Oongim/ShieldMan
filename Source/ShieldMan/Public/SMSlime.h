// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Character.h"

#include "SMSlime.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle			UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DeplayName = "MoveToTarget"),
	EMS_RunAway			UMETA(DeplayName = "RunAway"),
	EMS_Max				UMETA(DeplayName = "Max"),
};

UCLASS()
class SHIELDMAN_API ASMSlime : public APawn
{
	GENERATED_BODY()
private:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class AMetaBall_Slime* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
		float speedPower;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
		float Health;

	FTimerHandle RepeatTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
	float RepeatInterval;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		EEnemyMovementStatus status;


public:
	// Sets default values for this character's properties
	ASMSlime();

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = AI)
		const float GetHealth();

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus stat) { status = stat; }

	void OnRepeatTimer();



};
