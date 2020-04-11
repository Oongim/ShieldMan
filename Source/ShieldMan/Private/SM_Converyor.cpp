// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Converyor.h"
#include "ShieldManCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASM_Converyor::ASM_Converyor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = On_Collision;
	bisOverlap = true;
	Speed = 200.f;

}

// Called when the game starts or when spawned
void ASM_Converyor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASM_Converyor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bisOverlap)
	{
		FVector dir{ 0.f,-1.f,0.f };

		TArray<AActor*> OverlapObject;


		On_Collision->GetOverlappingActors(OverlapObject);

		for (int i = 0; i < OverlapObject.Num(); ++i)
			OverlapObject[i]->AddActorWorldOffset(dir * (DeltaTime * Speed));
	}
}


void ASM_Converyor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bisOverlap = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void ASM_Converyor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bisOverlap = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
}



void ASM_Converyor::SetCollisionFromBP(UBoxComponent* Col)
{

	On_Collision = Col;

	On_Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_Converyor::OnOverlapBegin);
	On_Collision->OnComponentEndOverlap.AddDynamic(this, &ASM_Converyor::OnOverlapEnd);
}