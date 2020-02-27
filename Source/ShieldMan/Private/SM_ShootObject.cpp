// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ShootObject.h"

#include "ShieldManCharacter.h"
#include "SM_ShootObjectBullet.h"

// Sets default values
ASM_ShootObject::ASM_ShootObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShootBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SHOOT_BODY"));

	RootComponent = ShootBody;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHOOTBODY(TEXT(
		"/Game/1Stage/AttackObject/SM_AttackObject.SM_AttackObject"));

	if (SM_SHOOTBODY.Succeeded())
	{
		ShootBody->SetStaticMesh(SM_SHOOTBODY.Object);
	}

	sleepTime = 0;
	maxSleepTime = 0.2f;
}

// Called when the game starts or when spawned
void ASM_ShootObject::BeginPlay()
{
	Super::BeginPlay();
	auto p = GetWorld()->GetPawnIterator();

	Player = Cast<AShieldManCharacter>(*p);

	SpawnBullet(GetActorLocation(), GetActorRotation());

}

// Called every frame
void ASM_ShootObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bFire) {
		FRotator ToPlayerInterpRot = 
			FMath::RInterpTo(GetActorRotation(), 
			(GetActorLocation() - Player->GetActorLocation()).Rotation(), DeltaTime, 2.f);
		SetActorRotation(ToPlayerInterpRot);
	}
	else
	{
		sleepTime += DeltaTime;
		if (sleepTime >= maxSleepTime)
		{
			bFire = false;
			sleepTime = 0;
		}
	}
	if (!SpawnPawn->IsAlive())
	{
		SpawnPawn->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());
		SpawnPawn->Init();
		bFire = true;
	}
}

void ASM_ShootObject::SpawnBullet(FVector Loc, FRotator Rot)
{
	SpawnPawn =
		GetWorld()->SpawnActor<ASM_ShootObjectBullet>(SpawnBulletClass, Loc, Rot);

}

