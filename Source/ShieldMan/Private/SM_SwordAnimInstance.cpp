// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_SwordAnimInstance.h"

USM_SwordAnimInstance::USM_SwordAnimInstance()
{

	isHiding = true;
	isGuard = false;
	AttackType = 0;
}

void USM_SwordAnimInstance::SetAttackType(int type)
{
	AttackType = type;
}

int USM_SwordAnimInstance::GetAttackType()
{
	return AttackType;
}

void USM_SwordAnimInstance::SetHiding(bool hiding)
{
	isHiding = hiding;
}
void USM_SwordAnimInstance::SetGuard(bool guard)
{
	isGuard = guard;
}
