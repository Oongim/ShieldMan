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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true),Replicated)
		FVector V_Hand_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true), Replicated)
		FVector V_Hand_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true))
		FRotator R_Hand_Right;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true))
		FRotator R_Hand_Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawn, Meta = (AllowPrivateAccess = true))
		bool bIsInAir;

public:
	USMAnimInstance();
	//UFUNCTION(NetMulticast, Reliable)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void ClampLHand();
	void ClampRHand();

public:
	void AddHand_RightPos(FVector pos);
	void AddHand_LeftPos(FVector pos);

	UFUNCTION(BlueprintCallable)
	void SetHand_RightPos(FVector pos);
	UFUNCTION(BlueprintCallable)
	void SetHand_LeftPos(FVector pos);

	void AddHand_RightRot(FRotator rot);
	void AddHand_LeftRot(FRotator rot);

	void SetHand_RightRot(FRotator rot);
	void SetHand_LeftRot(FRotator rot);

	const FVector GetHand_RightPos();
	const FVector GetHand_LeftPos();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

};
