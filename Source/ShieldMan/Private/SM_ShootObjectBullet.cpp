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
	Collision->SetupAttachment(Bullet);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BULLET(TEXT(
		"/Game/1Stage/AttackObject/SM_Bullet.SM_Bullet"));
	
	if (SM_BULLET.Succeeded())
	{
		Bullet->SetStaticMesh(SM_BULLET.Object);
	}

	count = 0;
	bAlive = true;
}

// Called when the game starts or when spawned
void ASM_ShootObjectBullet::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_ShootObjectBullet::OnOverlapBegin);
}

// Called every frame
void ASM_ShootObjectBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bAlive) {
		FHitResult HitResult;
		AddActorLocalOffset(FVector(-10.f,0.f,0.f), false, &HitResult);
		count++;
		if (count > 500)
			bAlive = false;
	}
}

bool ASM_ShootObjectBullet::IsAlive()
{

	return bAlive;
}

void ASM_ShootObjectBullet::Init()
{
	count = 0;
	bAlive = true;
}

void ASM_ShootObjectBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bAlive = false;
}

