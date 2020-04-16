// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_SpawnToPointArea.h"
#include "ShieldManCharacter.h"

// Sets default values
ASM_SpawnToPointArea::ASM_SpawnToPointArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AREABOX"));
	RootComponent = AreaBox;

	SpawnPoint = FVector(0.f);
	isMoving = false;

	speed = 1000.f;
}

// Called when the game starts or when spawned
void ASM_SpawnToPointArea::BeginPlay()
{
	Super::BeginPlay();

	AreaBox->OnComponentBeginOverlap.AddDynamic(this, &ASM_SpawnToPointArea::OnOverlapBegin);
}

void ASM_SpawnToPointArea::Tick(float DeltaTime)
{
	if (isMoving) {
		Super::Tick(DeltaTime);
		FVector SpareLength = Destination - CurrentPos;
		if (SpareLength.Z > 0.f) {
			CurrentPos += FVector{0.f,0.f,Direction.Y*speed * DeltaTime };
		}
		else if(SpareLength.Y>0.f){
			CurrentPos += FVector{ Direction.X * speed * DeltaTime ,Direction.Y * speed * DeltaTime ,0.f };
		}
		else
		{
			isMoving = false;
		}

		SMCharacter->SetActorLocation(CurrentPos);
	}
}

void ASM_SpawnToPointArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isMoving) {
		SMCharacter = Cast<AShieldManCharacter>(OtherActor);

		if (nullptr != SMCharacter)
		{
			SMCharacter->SetCharacterStatus(CharacterStatus::ImpossibleMove);

			Destination = GetActorLocation() + SpawnPoint;
			CurrentPos = SMCharacter->GetActorLocation();
			Direction = (GetActorLocation() + SpawnPoint - SMCharacter->GetActorLocation());
			Direction.Normalize(1.0f);
			isMoving = true;
		}
		else
		{

		}
	}
}

