// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Stage2_SlimePhase.h"
#include "MetaBall_Slime.h"
#include "SM_Stage2_Door.h"

// Sets default values
ASM_Stage2_SlimePhase::ASM_Stage2_SlimePhase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_SlimeArr.Reserve(5);

	OpenCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OPEN_COLLISION"));

	StartCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("START_COLLISION"));

	CeilingRock= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CEILING_ROCK"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CEILING(TEXT(
		"/Game/2Stage/SM_CeilingRock1.SM_CeilingRock1"));

	if (SM_CEILING.Succeeded())
	{
		CeilingRock->SetStaticMesh(SM_CEILING.Object);
	}
	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void ASM_Stage2_SlimePhase::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.SetTickFunctionEnable(false);
	StartCollision->SetWorldLocation(FVector(950.f, 260.f, 100.f));
	CeilingRock->SetWorldLocation(FVector(-300.f, 260.f, 1500.f));
	if (HasAuthority()) {
		ServerSpawnActor();
	}

	OpenCollision->OnComponentBeginOverlap.AddDynamic(this, &ASM_Stage2_SlimePhase::OnOpenOverlapBegin);
	StartCollision->OnComponentBeginOverlap.AddDynamic(this, &ASM_Stage2_SlimePhase::OnStartOverlapBegin);

	bOpened = false;
}

// Called every frame
void ASM_Stage2_SlimePhase::Tick(float DeltaTime)
{
	if (Role == ROLE_Authority)
	{
		Super::Tick(DeltaTime);
		
		ServerAddCeilingOffset(-10.f * DeltaTime);

		bool ended = false;
		for (auto slime : m_SlimeArr)
		{
			if (slime->GetAlive()) {
				ended = true;
				break;
			}
		}
		if (!ended) {
			Exit->OpenDoor();
			PrimaryActorTick.SetTickFunctionEnable(false);
			if (m_SlimeArr[0] != nullptr) {
				for (auto slime : m_SlimeArr)
				{
					slime->Destroy();
				}
			}
		}
	}
}

void ASM_Stage2_SlimePhase::OnOpenOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{
		ServerOpenDoor();
	}
}

void ASM_Stage2_SlimePhase::OnStartOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role == ROLE_Authority)
	{
		ServerCloseDoor();
	}
}

void ASM_Stage2_SlimePhase::ServerOpenDoor_Implementation()
{
	if (bOpened) return;
	if(nullptr != Entrance)
		Entrance->OpenDoor();
	for (auto slime : m_SlimeArr)
	{
		slime->MoveStart();
	}
	bOpened = true;
}

void ASM_Stage2_SlimePhase::ServerCloseDoor_Implementation()
{
	if (nullptr != Entrance)
		Entrance->CloseDoor();
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void ASM_Stage2_SlimePhase::ServerAddCeilingOffset_Implementation(float val)
{
	CeilingRock->AddLocalOffset(FVector(0.f, 0.f, -val));
}

void ASM_Stage2_SlimePhase::ServerSpawnActor_Implementation()
{
	m_SlimeArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Slime>(SlimeClass, FVector(-1000.f, -450.f, 10.f), FRotator::ZeroRotator));
	m_SlimeArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Slime>(SlimeClass, FVector(600.f, -450.f, 10.f), FRotator::ZeroRotator));
	m_SlimeArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Slime>(SlimeClass, FVector(600.f, 1000.f, 10.f), FRotator::ZeroRotator));
	m_SlimeArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Slime>(SlimeClass, FVector(-1000.f, 1000.f, 10.f), FRotator::ZeroRotator));
	m_SlimeArr.Emplace(GetWorld()->SpawnActor<AMetaBall_Slime>(SlimeClass, FVector(-600.f, 250.f, 10.f), FRotator::ZeroRotator));

	Entrance = GetWorld()->SpawnActor<ASM_Stage2_Door>(DoorClass, FVector(1060.f, 260.f, -61.f), FRotator::ZeroRotator);
	Exit = GetWorld()->SpawnActor<ASM_Stage2_Door>(DoorClass, FVector(-1582.f, 260.f, -61.f), FRotator::ZeroRotator);

}
