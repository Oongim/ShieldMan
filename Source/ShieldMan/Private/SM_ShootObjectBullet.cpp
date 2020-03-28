// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ShootObjectBullet.h"
#include "..\Public\SM_ShootObjectBullet.h"
#include "ShieldManCharacter.h"

// Sets default values
ASM_ShootObjectBullet::ASM_ShootObjectBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BULLET"));

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->SetSphereRadius(10.f);

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
	Bullet->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
	Collision->SetCollisionProfileName(TEXT("Shootbullet"));

	DeathMaxCount = 3.f;

	LifeTime = 10.f;

	bDead = false;

	static ConstructorHelpers::FClassFinder<AShieldManCharacter>CHARACTERCLASS(TEXT(
		"/Game/BP/BP_SMCharacter.BP_SMCharacter_C"
	));
	characterClass = CHARACTERCLASS.Class;
	
}

// Called when the game starts or when spawned
void ASM_ShootObjectBullet::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_ShootObjectBullet::OnOverlapBegin);
	
	Bullet->SetSimulatePhysics(true);
	

	GetWorldTimerManager().SetTimer(LifeTimerHandle,this,&ASM_ShootObjectBullet::Death, LifeTime);
}

void ASM_ShootObjectBullet::AddFroce(float power)
{
	Bullet->AddForce(GetActorRotation().Vector() * -power);
}

void ASM_ShootObjectBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bDead) return;

	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ASM_ShootObjectBullet::Death, DeathMaxCount);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetClass()->GetName());

	if (OtherActor->GetClass()->GetName() == characterClass->GetName())
	{
		auto character = Cast<AShieldManCharacter>(OtherActor);

		character->AddForceToCharacter(GetActorRotation().Vector(), this->GetVelocity().Size());
		
		ULog::Number(this->GetVelocity().Size(), "Size: ","",LO_Viewport);
	}

	bDead = true;
}

void ASM_ShootObjectBullet::Death()
{
	Destroy();
}

