// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"
#include "Components/BillboardComponent.h"
#include "ShieldManCharacter.h"
#include "SM_GameInstance.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = TransitionVolume;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	TransitionLevelName = "Stage2";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();

	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
	
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		auto Character = Cast<AShieldManCharacter>(OtherActor);
		if (Character)
		{

		/*	UWorld* World = GetWorld();
			if (!ensure(World != nullptr)) return;
			World->ServerTravel("/Game/2Stage/Stage2?listen");*/

			ChangeStage();
			//auto GI = Cast<USM_GameInstance>(GetGameInstance());
			//GI->ChangeStage(TransitionLevelName);
			//Character->SwitchLevel(TransitionLevelName);
		}

	}
}

