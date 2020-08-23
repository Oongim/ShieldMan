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

	AttackMagicCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MAGIC_CIRCLE"));

	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);
	AttackMagicCircle->SetupAttachment(RootComponent);

	AttackMagicCircle->SetCollisionProfileName(TEXT("NoCollision"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ATTACKCIECLE(TEXT(
			"/Game/Import/MagicCircle.MagicCircle"));

	if (SM_ATTACKCIECLE.Succeeded())
	{
		AttackMagicCircle->SetStaticMesh(SM_ATTACKCIECLE.Object);
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
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);

	Player = Cast<AShieldManCharacter>(FoundActors[0]);
	
	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called every frame
void ASM_ShootObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bFire) {
		FVector Player_Location = Player->GetActorLocation();
		Player_Location.Z += 30.f;
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
	if (reloadTime >= reloadMaxTime)
	{
		AttackMagicCircle->SetScalarParameterValueOnMaterials(FName("Opacity"), 1.0f);
		reloadTime = 0.f;
		SpawnBullet(GetActorLocation(), GetActorRotation());
		bFire = true;
	}
	else if (reloadTime <= 0.5f) {
		AttackMagicCircle->SetScalarParameterValueOnMaterials(FName("Opacity"), 1-reloadTime*2);
	}
	else if (reloadTime <= 0.8f) {
		AttackMagicCircle->SetScalarParameterValueOnMaterials(FName("Opacity"), 0.f);
	}
	else if (reloadTime >= reloadMaxTime - 1.0f) {
		AttackMagicCircle->SetScalarParameterValueOnMaterials(FName("Opacity"), reloadTime-(int)reloadTime);
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

