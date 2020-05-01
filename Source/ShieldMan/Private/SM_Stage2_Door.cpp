// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Stage2_Door.h"

// Sets default values
ASM_Stage2_Door::ASM_Stage2_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Door_Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DOOR_FRAME"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DOOR"));

	RootComponent = Door_Frame;
	Door->SetupAttachment(RootComponent);

	bMoveDoor = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called when the game starts or when spawned
void ASM_Stage2_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASM_Stage2_Door::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);

		fDoorDeltaTime += DeltaTime;

		float fDoorHeight = 220 * DeltaTime;

		Door->AddLocalOffset(FVector(0.f, 0.f, -fDoorHeight));
		if (fDoorDeltaTime >= 0.99f) {

			PrimaryActorTick.SetTickFunctionEnable(false);
		}
}

void ASM_Stage2_Door::ToggleDoor()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
}

