// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ShootObjectBullet.h"
#include "..\Public\SM_ShootObjectBullet.h"

// Sets default values
ASM_ShootObjectBullet::ASM_ShootObjectBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BULLET"));

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->SetSphereRadius(8.f);

	RootComponent = Bullet;
	Collision->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BULLET(TEXT(
		"/Game/1Stage/AttackObject/SM_Bullet.SM_Bullet"));
	
	if (SM_BULLET.Succeeded())
	{
		Bullet->SetStaticMesh(SM_BULLET.Object);
	}
	//bAlive = true;
	
	Bullet->SetSimulatePhysics(true);
	Bullet->SetCollisionProfileName(TEXT("Shootbullet"));

	DeathMaxCount = 3.f;
	bDeath = false;

	LifeTime = 0.f;
}

// Called when the game starts or when spawned
void ASM_ShootObjectBullet::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_ShootObjectBullet::OnOverlapBegin);
	
	Bullet->SetSimulatePhysics(true);
	Bullet->AddForce(GetActorRotation().Vector() * -200000);
}

// Called every frame
void ASM_ShootObjectBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDeath)
	{
		DeathCount += DeltaTime;
		if (DeathCount >= DeathMaxCount)
			Destroy();
	}
	else {
		LifeTime += DeltaTime;
		if (LifeTime >= 10.f)
			Destroy();
	}
}

//bool ASM_ShootObjectBullet::IsAlive()
//{
//
//	return bAlive;
//}
//
//void ASM_ShootObjectBullet::Init()
//{
//	//count = 0;
//	bAlive = true;
//	
//	Bullet->AddForce(GetActorRotation().Vector() * -200000);
//
//}

void ASM_ShootObjectBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bDeath = true;
}

