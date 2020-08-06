// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Ghost_AttackObject.h"
#include "MetaBall_Ghost.h"
#include "ShieldManCharacter.h"

// Sets default values
ASM_Ghost_AttackObject::ASM_Ghost_AttackObject()
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

	Bullet->SetSimulatePhysics(true);
	Bullet->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
	Collision->SetCollisionProfileName(TEXT("GhostBulletStart"));

	DeathMaxCount = 3.f;

	LifeTime = 30.f;

	bDead = false;
	bDefended = false;
	static ConstructorHelpers::FClassFinder<AShieldManCharacter>CHARACTERCLASS(TEXT(
		"/Game/BP/BP_SMCharacter.BP_SMCharacter_C"
	));
	characterClass = CHARACTERCLASS.Class;

	EndEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("END_EFFECT"));
	MoveEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MOVE_EFFECT"));

	MoveEffect->SetupAttachment(RootComponent);
	MoveEffect->SetRelativeScale3D(FVector{ 0.5f,0.5f,0.5f });
	MoveEffect->SetRelativeLocation(FVector{ -12.f,0.f,-7.f });

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/InfinityBladeEffects/Effects/FX_Archive/P_Posseser_Move.P_Posseser_Move"));

	if (ParticleAsset.Succeeded())
	{
		MoveEffect->SetTemplate(ParticleAsset.Object);
	}

}

// Called when the game starts or when spawned
void ASM_Ghost_AttackObject::BeginPlay()
{
	Super::BeginPlay();
	bDefended = false;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_Ghost_AttackObject::OnOverlapBegin);
	Bullet->OnComponentHit.AddDynamic(this, &ASM_Ghost_AttackObject::OnHit);

	Bullet->SetSimulatePhysics(true);

	Bullet->AddForce(GetActorRotation().Vector() * 20000);

	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &ASM_Ghost_AttackObject::Death, LifeTime);
}


void ASM_Ghost_AttackObject::SetTarget(AActor* actor)
{
	Target = actor;
}

void ASM_Ghost_AttackObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bDead) return;
	bool bCollisioned = false;

	if (bDefended)
	{
		if (OtherActor->GetClass()->GetName() == "BP_MetaBall_Boss_C") {
			return;
		}
		if (OtherActor->GetClass()->GetName() == Target->GetClass()->GetName())
		{
			auto ghost = Cast<AMetaBall_Ghost>(OtherActor);
			if (ghost->GetAlive()) {
				ghost->Attacked();
				bCollisioned = true;
			}
		}
	}
	else if (OtherActor->GetClass()->GetName() == characterClass->GetName())
	{
		MoveEffect->SetVisibility(false);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EndEffect, GetActorLocation(), GetActorRotation(), true);
		auto character = Cast<AShieldManCharacter>(OtherActor);
		if (false == character->isDeath())
		{
			character->AddForceToCharacter(GetActorRotation().Vector() * -1, 20.f);
			character->DecreaseHP(20.f);
			bCollisioned = true;
		}
	}

	if (bCollisioned) {
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ASM_Ghost_AttackObject::Death, DeathMaxCount);
		bDead = true;
		Bullet->SetSimulatePhysics(false);
	}
}

void ASM_Ghost_AttackObject::Death()
{
	ULog::Invalid("Death", "", LO_Viewport);
	Destroy();
}

void ASM_Ghost_AttackObject::OnHit(UPrimitiveComponent* OnHittedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bDead) return;
	
	if (OtherActor == nullptr)return;
	ULog::Invalid(OtherComp->GetClass()->GetName(), "", LO_Viewport);
	if (OtherActor->GetClass()->GetName() == FString("SM_Shield")||
		OtherComp->GetClass()->GetName() == FString("BoxComponent"))
	{
		ULog::Invalid("OnHit", "", LO_Viewport);
		MoveEffect->SetVisibility(false);
		FVector target_velocity = Target->GetActorLocation() - GetActorLocation();
		target_velocity.Normalize();

		Bullet->AddForce(target_velocity * 100000);
		bDefended = true;
	}
	else
	{
		ULog::Invalid(OtherComp->GetName(), "", LO_Viewport);
		Death();
	}
}


