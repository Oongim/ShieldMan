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

	bClosing = false;
	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void ASM_Stage2_Door::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called every frame
void ASM_Stage2_Door::Tick(float DeltaTime)
{
	if (Role == ROLE_Authority) {
		Super::Tick(DeltaTime);
		fDoorDeltaTime += DeltaTime;
		float fDoorHeight = 220 * DeltaTime;


		if (bClosing) {
			ServerOpenDoor(-fDoorHeight);
			//Door->AddLocalOffset(FVector(0.f, 0.f, -fDoorHeight));
			if (fDoorDeltaTime >= 0.99f) {
				PrimaryActorTick.SetTickFunctionEnable(false);
				fDoorDeltaTime = 0.f;
			}
		}
		else
		{
			ServerOpenDoor(fDoorHeight);
			//Door->AddLocalOffset(FVector(0.f, 0.f, fDoorHeight));
			if (fDoorDeltaTime >= 0.99f) {
				PrimaryActorTick.SetTickFunctionEnable(false);
				fDoorDeltaTime = 0.f;
			}
		}
	}
}

void ASM_Stage2_Door::OpenDoor()
{
	if (!bClosing) {
		PrimaryActorTick.SetTickFunctionEnable(true);
		bClosing = true;
	}
}

void ASM_Stage2_Door::CloseDoor()
{
	if (bClosing) {
		PrimaryActorTick.SetTickFunctionEnable(true);
		bClosing = false;
	}
}


void ASM_Stage2_Door::ServerOpenDoor_Implementation(float val)
{
	Door->AddLocalOffset(FVector(0.f, 0.f, val));
}