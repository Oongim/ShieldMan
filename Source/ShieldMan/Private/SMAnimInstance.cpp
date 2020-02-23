// Fill out your copyright notice in the Description page of Project Settings.


#include "SMAnimInstance.h"

USMAnimInstance::USMAnimInstance()
{

}

void USMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ClampLHand();
	ClampRHand();
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

void USMAnimInstance::ClampLHand()
{
	Hand_Left.X=FMath::Clamp(Hand_Left.X, -110.f, 10.f);
	Hand_Left.Y=FMath::Clamp(Hand_Left.Y, -60.f, 60.f);
	Hand_Left.Z=FMath::Clamp(Hand_Left.Z, -80.f, 35.f);
}
void USMAnimInstance::ClampRHand()
{
	Hand_Right.X=FMath::Clamp(Hand_Right.X, -10.f, 110.f);
	Hand_Right.Y=FMath::Clamp(Hand_Right.Y, -60.f, 60.f);
	Hand_Right.Z=FMath::Clamp(Hand_Right.Z, -35.f, 80.f);
}