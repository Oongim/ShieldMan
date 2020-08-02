// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_LAVA.h"

#include "ShieldManCharacter.h"

// Sets default values
ASM_LAVA::ASM_LAVA()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	downfall = false;
	
}

// Called when the game starts or when spawned
void ASM_LAVA::BeginPlay()
{
	Super::BeginPlay();

	auto p = GetWorld()->GetPawnIterator();
	Player = Cast<AShieldManCharacter>(*p);
	PrimaryActorTick.SetTickFunctionEnable(false);

}

// Called every frame
void ASM_LAVA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASM_LAVA::SetCollisionFromBP(UBoxComponent* Col)
{
	downfallLAVA = Col;
	downfallLAVA->OnComponentBeginOverlap.AddDynamic(this, &ASM_LAVA::Begin_downfallLAVA);
}

void ASM_LAVA::Begin_downfallLAVA(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (downfall == false)
	{
		UE_LOG(LogTemp, Log, TEXT("Begin_downfallLAVA"));
		//Player->DecreaseHP(100.f);
		Player->ChangeMagmaDeath();
	}
}
