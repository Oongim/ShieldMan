// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "SM_SwordAnimInstance.generated.h"

// IDLE, TOPSLASH, TOPGUARD, MIDSLASH, MIDGUARD, BOTSLASH, BOTGUARD

enum ATTACK_TYPE {
	TOP = 0, MIDDLE, BOTTOM
};


/**
 *
 */
UCLASS()
class SHIELDMAN_API USM_SwordAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = state, Meta = (AllowPrivateAccess = true), Replicated)
		int AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = state, Meta = (AllowPrivateAccess = true), Replicated)
		bool isHiding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = state, Meta = (AllowPrivateAccess = true), Replicated)
		bool isGuard;


public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	void SetAttackType(int type);
	void SetHiding(bool hiding);
	void SetGuard(bool guard);
	int GetAttackType();
	USM_SwordAnimInstance();
};
