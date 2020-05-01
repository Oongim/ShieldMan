// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_SetShieldLaser.h"
#include "SM_Shield.h"
#include "ShieldManCharacter.h"
#include "SM_ShootObject.h"

// Sets default values
ASM_SetShieldLaser::ASM_SetShieldLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Lazer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LAZER"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));

	RootComponent = Lazer;
	Trigger->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_LAZER(TEXT(
		"/Game/1Stage/SetShieldLazer/SM_Lazer.SM_Lazer"));
	if (SM_LAZER.Succeeded())
	{
		Lazer->SetStaticMesh(SM_LAZER.Object);
	}
	Trigger->SetBoxExtent(FVector(100.f, 1.f, 1.f));
	
	Trigger->SetCollisionProfileName(TEXT("Lazer"));
	Lazer->SetCollisionProfileName(TEXT("NoCollision"));

	ShieldClass = ASM_Shield::StaticClass();

}


// Called when the game starts or when spawned
void ASM_SetShieldLaser::BeginPlay()
{
	Super::BeginPlay();
	ULog::Hello();
}

void ASM_SetShieldLaser::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASM_SetShieldLaser::OnCharacterOverlap);
}


void ASM_SetShieldLaser::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ULog::Hello();
	
	auto SMCharacter = Cast<AShieldManCharacter>(OtherActor);

	if (nullptr != SMCharacter && nullptr != ShieldClass)
	{
		if (SMCharacter->CanSetShield())
		{
			auto NewShieldR = GetWorld()->SpawnActor<ASM_Shield>(FVector::ZeroVector, FRotator::ZeroRotator);
			auto NewShieldL = GetWorld()->SpawnActor<ASM_Shield>(FVector::ZeroVector, FRotator::ZeroRotator);

			SMCharacter->ToggleAttackPossible();
			SMCharacter->SetShield(NewShieldR, NewShieldL);
			StartAttack();
		}
		else
		{
			//ULog::Invalid("Can't equip Shield", "", LO_Viewport);
		}
	}
	else
	{
		//ULog::Invalid("Can't OnCharacterOverlap start", "", LO_Viewport);
	}

	
}

void ASM_SetShieldLaser::StartAttack()
{
	ULog::Hello(LO_Viewport);

	for (auto shootObject : GetWorld()->GetCurrentLevel()->Actors)
	{
		
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASM_ShootObject::StaticClass(), FoundActors);

		for (auto d : FoundActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *d->GetName());
			auto shooter = Cast< ASM_ShootObject>(d);
			shooter->StartAttack();
		}
	}

}
