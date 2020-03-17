// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_SpawnToPointArea.h"
#include "ShieldManCharacter.h"

// Sets default values
ASM_SpawnToPointArea::ASM_SpawnToPointArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AREABOX"));
	RootComponent = AreaBox;

	SpawnPoint = FVector(0.f);
}

// Called when the game starts or when spawned
void ASM_SpawnToPointArea::BeginPlay()
{
	Super::BeginPlay();

	AreaBox->OnComponentBeginOverlap.AddDynamic(this, &ASM_SpawnToPointArea::OnOverlapBegin);
}

void ASM_SpawnToPointArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto SMCharacter = Cast<AShieldManCharacter>(OtherActor);

	if (nullptr != SMCharacter)
	{
		ULog::Invalid("Yes Character", "", LO_Viewport);
		SMCharacter->SetActorLocation(GetActorLocation() + SpawnPoint);
	}
	else
	{
		ULog::Invalid("No Character", "", LO_Viewport);
	}
}

