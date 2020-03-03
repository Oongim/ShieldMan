// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Shield.h"

// Sets default values
ASM_Shield::ASM_Shield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Shield;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SHIELD(TEXT(
		"/Game/Import/Shield.Shield"));
	if (SK_SHIELD.Succeeded())
	{
		Shield->SetSkeletalMesh(SK_SHIELD.Object);
	}

	Shield->SetCollisionProfileName(TEXT("NoCollision"));
	Shield->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
}

// Called when the game starts or when spawned
void ASM_Shield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASM_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

