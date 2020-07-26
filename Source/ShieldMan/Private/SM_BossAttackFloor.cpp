// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_BossAttackFloor.h"

// Sets default values
ASM_BossAttackFloor::ASM_BossAttackFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
}

// Called when the game starts or when spawned
void ASM_BossAttackFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASM_BossAttackFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

