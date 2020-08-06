// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_BossGhost_AttackObject.h"
#include "MetaBall_Boss.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ASM_BossGhost_AttackObject::SetTarget(AActor* actor)
{
	Super::SetTarget(actor);
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BossClass, Actors);
	Target = Actors[0];
}
