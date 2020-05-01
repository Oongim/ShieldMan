// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ShootObject.h"

#include "ShieldManCharacter.h"
#include "SM_ShootObjectBullet.h"

// Sets default values
ASM_ShootObject::ASM_ShootObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("COLLISION"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SHOOT_BODY"));

	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);

	Collision->SetCollisionProfileName(TEXT("NoCollision"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_SHOOTBODY(TEXT(
			"/Game/Import/Charlotte/Charlotte.Charlotte"));

	if (SM_SHOOTBODY.Succeeded())
	{
		Mesh->SetSkeletalMesh(SM_SHOOTBODY.Object);
	}

	sleepTime = 0;
	maxSleepTime = 0.2f;

	reloadMaxTime = 3; 
	reloadTime = FMath::Rand() % (int)reloadMaxTime;

	ShootPower = 200000;

}

// Called when the game starts or when spawned
void ASM_ShootObject::BeginPlay()
{
	Super::BeginPlay();
	auto p = GetWorld()->GetPawnIterator();

	Player = Cast<AShieldManCharacter>(*p);

	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called every frame
void ASM_ShootObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bFire) {
		FVector Player_Location = Player->GetActorLocation();
		Player_Location.Z += 50.f;
		FRotator ToPlayerInterpRot = 
			FMath::RInterpTo(GetActorRotation() , (GetActorLocation() - Player_Location).Rotation(), DeltaTime, 3.f);
		SetActorRotation(ToPlayerInterpRot );
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
	reloadTime += DeltaTime;

	if (reloadTime>=reloadMaxTime)
	{
		reloadTime = 0.f;
		SpawnBullet(GetActorLocation(), GetActorRotation());
		bFire = true;
	}
}

void ASM_ShootObject::SpawnBullet(FVector Loc, FRotator Rot)
{
	SpawnPawn = GetWorld()->SpawnActor<ASM_ShootObjectBullet>(SpawnBulletClass, Loc, Rot);
	SpawnPawn->AddFroce(ShootPower);
}

void ASM_ShootObject::StartAttack()
{
	if (!PrimaryActorTick.IsTickFunctionEnabled()) {
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}

void ASM_ShootObject::StopAttack()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
}

