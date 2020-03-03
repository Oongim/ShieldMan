// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "Animation/AnimInstance.h"
#include "SMAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API USMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true))
		FVector Hand_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true))
		FVector Hand_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true))
		bool bIsInAir;

public:
	USMAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void ClampLHand();
	void ClampRHand();

public:
	void AddHand_RightPos(FVector pos);
	void AddHand_LeftPos(FVector pos);

	void SetHand_RightPos(FVector pos);
	void SetHand_LeftPos(FVector pos);

	const FVector GetHand_RightPos();
	const FVector GetHand_LeftPos();
};
