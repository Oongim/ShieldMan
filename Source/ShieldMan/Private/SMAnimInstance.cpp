// Fill out your copyright notice in the Description page of Project Settings.


#include "SMAnimInstance.h"
#include "Net/UnrealNetwork.h"

USMAnimInstance::USMAnimInstance()
{
	bIsInAir = false;  
}

void USMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ClampLHand();
	ClampRHand();
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			bIsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}

}
void USMAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USMAnimInstance, V_Hand_Right);
	DOREPLIFETIME(USMAnimInstance, V_Hand_Left);
}

void USMAnimInstance::AddHand_RightPos(FVector pos)
{
	V_Hand_Right += pos;
	
}

void USMAnimInstance::AddHand_LeftPos(FVector pos)
{
	V_Hand_Left += pos;
	
}

void USMAnimInstance::SetHand_RightPos(FVector pos)
{
	V_Hand_Right = pos;

}

void USMAnimInstance::SetHand_LeftPos(FVector pos) 
{
	V_Hand_Left = pos;

}

void USMAnimInstance::AddHand_RightRot(FRotator rot)
{
	R_Hand_Right += rot;
}

void USMAnimInstance::AddHand_LeftRot(FRotator rot)
{
	R_Hand_Left += rot;
}

void USMAnimInstance::SetHand_RightRot(FRotator rot)
{
	R_Hand_Right = rot;
}

void USMAnimInstance::SetHand_LeftRot(FRotator rot)
{
	R_Hand_Left = rot;
}

const FVector USMAnimInstance::GetHand_RightPos()
{
	return V_Hand_Right;
}

const FVector USMAnimInstance::GetHand_LeftPos()
{
	return V_Hand_Left;
}

void USMAnimInstance::ClampLHand()
{
	V_Hand_Left.X = FMath::Clamp(V_Hand_Left.X, -95.f, 0.f);
	V_Hand_Left.Y = FMath::Clamp(V_Hand_Left.Y, -45.f, 45.f);
	V_Hand_Left.Z = FMath::Clamp(V_Hand_Left.Z, -130.f, 100.f);
}
void USMAnimInstance::ClampRHand()
{
	V_Hand_Right.X = FMath::Clamp(V_Hand_Right.X, -95.f, 0.f);
	V_Hand_Right.Y = FMath::Clamp(V_Hand_Right.Y, -100.f, 100.f);
	V_Hand_Right.Z = FMath::Clamp(V_Hand_Right.Z, -130.f, 100.f);
}