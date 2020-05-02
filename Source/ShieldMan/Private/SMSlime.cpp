// Fill out your copyright notice in the Description page of Project Settings.


#include "SMSlime.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MetaBall_Slime.h"
// Sets default values


ASMSlime::ASMSlime()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Actor = CreateDefaultSubobject<AMetaBall_Slime>(TEXT("ACTOR"));

	Health = 100.0f;

	speedPower = 1.f;

	RepeatInterval = 3.0f;

	if(nullptr == AIControllerClass)UE_LOG(LogTemp, Warning, TEXT("NotNNN!!"));

	UE_LOG(LogTemp, Warning, TEXT("ASMSlime"));
}

void ASMSlime::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ASMSlime::OnRepeatTimer, RepeatInterval, true);


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

void ASMSlime::OnRepeatTimer()
{
	//auto CurrentPawn = Cast<AMetaBall_Slime>(Actor);
	if(nullptr== Actor)ULog::Invalid("Actor", "", LO_Viewport);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, NextLocation))
	{
		//NextLocation.Location.Z = 0.f;
		//CurrentPawn->AddForceToVelocity(NextLocation.Location, speedPower*10000);
		Actor->AddForceToVelocity(NextLocation.Location, speedPower * 10000);
		ULog::Vector(NextLocation.Location, "MyActor location: ", "", LO_Viewport);
	}
}
