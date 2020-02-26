// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ShootObject.h"
#include "ShieldManCharacter.h"

// Sets default values
ASM_ShootObject::ASM_ShootObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShootBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SHOOT_BODY"));

	RootComponent = ShootBody;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHOOTBODY(TEXT(
		"/Game/1Stage/AttackObject/SM_AttackObject.SM_AttackObject"));

	if (SM_SHOOTBODY.Succeeded())
	{
		ShootBody->SetStaticMesh(SM_SHOOTBODY.Object);
	}

}

// Called when the game starts or when spawned
void ASM_ShootObject::BeginPlay()
{
	Super::BeginPlay();
	auto p = GetWorld()->GetPawnIterator();

	Player = Cast<AShieldManCharacter>(*p);
}

// Called every frame
void ASM_ShootObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ULog::Vector(Player->GetActorLocation(), "The Number is: ", "", LO_Viewport);
}

