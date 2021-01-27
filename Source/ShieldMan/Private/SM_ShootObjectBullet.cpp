// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ShootObjectBullet.h"
#include "ShieldManCharacter.h"

// Sets default values
ASM_ShootObjectBullet::ASM_ShootObjectBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BULLET"));

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->SetSphereRadius(50.f);

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
	//Bullet->SetNotifyRigidBodyCollision(true);
	Bullet->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
	Collision->SetCollisionProfileName(TEXT("Shootbullet"));

	DeathMaxCount = 3.f;

	LifeTime = 30.f;

	bDead = false;

	static ConstructorHelpers::FClassFinder<AShieldManCharacter>CHARACTERCLASS(TEXT(
		"/Game/BP/BP_SMCharacter.BP_SMCharacter_C"
	));
	characterClass = CHARACTERCLASS.Class;
	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void ASM_ShootObjectBullet::BeginPlay()
{
	if (HasAuthority()) {
		Super::BeginPlay();

		Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_ShootObjectBullet::OnOverlapBegin);
		Bullet->OnComponentHit.AddDynamic(this, &ASM_ShootObjectBullet::OnHit);

		Bullet->SetSimulatePhysics(true);

		GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &ASM_ShootObjectBullet::Death, LifeTime);
	}
}

void ASM_ShootObjectBullet::AddFroce(float power)
{
	Bullet->AddForce(GetActorRotation().Vector() * -power);
}

void ASM_ShootObjectBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{
		ServerOverlapBegin(OtherActor);
	}
}

void ASM_ShootObjectBullet::ServerOverlapBegin_Implementation(AActor* OtherActor)
{
	if (bDead) return;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ASM_ShootObjectBullet::Death, DeathMaxCount);

	if (OtherActor->GetClass()->GetName() == characterClass->GetName())
	{
		auto character = Cast<AShieldManCharacter>(OtherActor);
		character->AddForceToCharacter(GetActorRotation().Vector(), this->GetVelocity().Size() * 10);
	}
	bDead = true;
	StartNiagaraEffect();
}

void ASM_ShootObjectBullet::ServerStartNiagaraEffect_Implementation()
{
	if (bDead) return;
	StartNiagaraEffect();
}

void ASM_ShootObjectBullet::Death()
{
	Destroy();
}

void ASM_ShootObjectBullet::OnHit(UPrimitiveComponent* OnHittedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority)
	{
		ServerStartNiagaraEffect();
	}
}



