// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_PoseCheck.h"

// Sets default values
ASM_PoseCheck::ASM_PoseCheck()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PoseCheck = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("POSE_CHECK"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DOOR"));

	//Right_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("RIGHT_COLLISON"));
	//Left_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("LEFT_COLLISON"));

	Right_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("RIGHT_LIGHT"));
	Left_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LEFT_LIGHT"));
	
	
	RootComponent = PoseCheck;
	Door->SetupAttachment(PoseCheck);
	//Right_Collision->SetupAttachment(PoseCheck);
	//Left_Collision->SetupAttachment(PoseCheck);
	//Right_Light->SetupAttachment(Right_Collision);
	//Left_Light->SetupAttachment(Left_Collision);

	Door->SetRelativeLocation(FVector(-14.f, 0.f, 0.f));
	Right_Light->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Left_Light->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	//StaticMesh ¼³Á¤
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOOR(TEXT(
		"/Game/1Stage/PoseCheck/SM_Door.SM_Door"));
	if (SM_DOOR.Succeeded())
	{
		Door->SetStaticMesh(SM_DOOR.Object);
	}

}

// Called when the game starts or when spawned
void ASM_PoseCheck::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASM_PoseCheck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

