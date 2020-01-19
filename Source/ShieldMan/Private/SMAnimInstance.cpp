// Fill out your copyright notice in the Description page of Project Settings.


#include "SMAnimInstance.h"

USMAnimInstance::USMAnimInstance()
{

}

void USMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void USMAnimInstance::AddHand_RightPos(FVector pos)
{
	Hand_Right += pos;
}

void USMAnimInstance::AddHand_LeftPos(FVector pos)
{
	Hand_Left += pos;
}

void USMAnimInstance::SetHand_RightPos(FVector pos)
{
	Hand_Right = pos;
}

void USMAnimInstance::SetHand_LeftPos(FVector pos) 
{
	Hand_Left = pos;
}

const FVector USMAnimInstance::GetHand_RightPos()
{
	return Hand_Right;
}

const FVector USMAnimInstance::GetHand_LeftPos()
{
	return Hand_Left;
}
