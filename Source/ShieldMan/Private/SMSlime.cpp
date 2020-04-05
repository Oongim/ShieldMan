// Fill out your copyright notice in the Description page of Project Settings.


#include "SMSlime.h"
#include "GameFramework/Pawn.h"
// Sets default values


ASMSlime::ASMSlime()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0f;
	//GetMovementComponent()->NavAgentProps.AgentHeight = 320.f;
	//GetMovementComponent()->NavAgentProps.AgentRadius = 160.f;

}



float ASMSlime::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Health -= Damage;

	return Damage;
}

const float ASMSlime::GetHealth()
{
	return Health;
}