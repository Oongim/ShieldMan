// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_FloatingMoveFlatform.h"

// Sets default values
ASM_FloatingMoveFlatform::ASM_FloatingMoveFlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	bInterping = false;

	InterpSpeed = 4.0f;
	InterpTime = 1.0f;

}

// Called when the game starts or when spawned
void ASM_FloatingMoveFlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	bInterping = false;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &ASM_FloatingMoveFlatform::ToggleInterping, InterpTime);

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void ASM_FloatingMoveFlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		FVector CurrentLocation = GetActorLocation();

		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

		SetActorLocation(Interp);


		float DistanceTraveld = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveld <= 10.f)
		{
			ToggleInterping();

			GetWorldTimerManager().SetTimer(InterpTimer, this, &ASM_FloatingMoveFlatform::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}

	
}

void ASM_FloatingMoveFlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void ASM_FloatingMoveFlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}

