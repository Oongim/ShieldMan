// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Stage2_GhostPhase.h"
#include "MetaBall_Ghost.h"
#include "SM_Stage2_Door.h"

// Sets default values
ASM_Stage2_GhostPhase::ASM_Stage2_GhostPhase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_GhostArr.Reserve(5);

	OpenCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OPEN_COLLISION"));

	StartCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("START_COLLISION"));

	
}

// Called when the game starts or when spawned
void ASM_Stage2_GhostPhase::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(false);
	StartCollision->SetWorldLocation(FVector(-2520.f, 260.f, -61.f));

	Entrance = GetWorld()->SpawnActor<ASM_Stage2_Door>(DoorClass, FVector(-2420.f, 260.f, -61.f), FRotator::ZeroRotator);
	Exit = GetWorld()->SpawnActor<ASM_Stage2_Door>(DoorClass, FVector(-4000.f, 260.f, -61.f), FRotator::ZeroRotator);

	OpenCollision->OnComponentBeginOverlap.AddDynamic(this, &ASM_Stage2_GhostPhase::OnOpenOverlapBegin);
	StartCollision->OnComponentBeginOverlap.AddDynamic(this, &ASM_Stage2_GhostPhase::OnStartOverlapBegin);

	bOpened = false;
}

// Called every frame
void ASM_Stage2_GhostPhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool ended = false;
	for (auto ghost : m_GhostArr)
	{
		if (ghost->GetAlive()) {
			ended = true;
			break;
		}
	}
	if (!ended) {
		Exit->OpenDoor();
		PrimaryActorTick.SetTickFunctionEnable(false);
		if (m_GhostArr[0] != nullptr) {
			for (auto ghost : m_GhostArr)
			{
				ghost->Destroy();
			}
		}
	}
}

void ASM_Stage2_GhostPhase::OnOpenOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bOpened)return;
	Entrance->OpenDoor();
	m_GhostArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Ghost>(GhostClass, FVector(-3600.f, 1000.f, 250.f), FRotator::ZeroRotator));
	m_GhostArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Ghost>(GhostClass, FVector(-2600.f, 1000.f, 250.f), FRotator::ZeroRotator));
	m_GhostArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Ghost>(GhostClass, FVector(-3100.f, 260.f, 250.f), FRotator::ZeroRotator));
	m_GhostArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Ghost>(GhostClass, FVector(-3600.f, -600.f, 250.f), FRotator::ZeroRotator));
	m_GhostArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Ghost>(GhostClass, FVector(-2600.f, -600.f, 250.f), FRotator::ZeroRotator));

	for (auto ghost : m_GhostArr)
	{
		ghost->MoveStart();
	}
	bOpened = true;
}

void ASM_Stage2_GhostPhase::OnStartOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Entrance->CloseDoor();
	PrimaryActorTick.SetTickFunctionEnable(true);
}

