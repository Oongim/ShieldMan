// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_PoseCheck.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASM_PoseCheck::ASM_PoseCheck()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PoseCheck = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("POSE_CHECK"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DOOR"));

	Right_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("RIGHT_LIGHT"));
	Left_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LEFT_LIGHT"));
	Answer_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Answer_LIGHT"));


	RootComponent = PoseCheck;
	Door->SetupAttachment(PoseCheck);
	Answer_Light->SetupAttachment(PoseCheck);

	Door->SetRelativeLocation(FVector(-14.f, 0.f, 0.f));

	//라이트 설정
	Right_Light->bVisible = false;
	Right_Light->LightColor = FColor(0, 0, 255, 1);
	Left_Light->bVisible = false;
	Left_Light->LightColor = FColor(255, 0, 0, 1);
	Answer_Light->bVisible = false;
	Answer_Light->LightColor = FColor(0, 255, 0, 1);
	Answer_Light->SetRelativeLocation(FVector(0.f, 0.f, 60.f));

	//StaticMesh 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOOR(TEXT(
		"/Game/1Stage/PoseCheck/SM_Door.SM_Door"));
	if (SM_DOOR.Succeeded())
	{
		Door->SetStaticMesh(SM_DOOR.Object);
	}

	//Door 이동설정
	fDoorDeltaTime = 0;
}

// Called when the game starts or when spawned
void ASM_PoseCheck::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called every frame
void ASM_PoseCheck::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	fDoorDeltaTime += DeltaTime;

	float fDoorHeight = 200 * DeltaTime;

	Door->AddLocalOffset(FVector(0.f, 0.f, -fDoorHeight));
	if (fDoorDeltaTime >= 0.99f) {

		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void ASM_PoseCheck::SetCollisionFromBP(UBoxComponent* Right, UBoxComponent* Left)
{
	Right_Collision = Right;
	Left_Collision = Left;

	//콜리전박스 이벤트 설정
	Right_Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_PoseCheck::OnRightOverlapBegin);
	Right_Collision->OnComponentEndOverlap.AddDynamic(this, &ASM_PoseCheck::OnRightOverlapEnd);
	Left_Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_PoseCheck::OnLeftOverlapBegin);
	Left_Collision->OnComponentEndOverlap.AddDynamic(this, &ASM_PoseCheck::OnLeftOverlapEnd);

	//콜리전박스의 위치로 라이트 이동
	Right_Light->SetRelativeLocation(Right_Collision->GetRelativeTransform().GetLocation());
	Left_Light->SetRelativeLocation(Left_Collision->GetRelativeTransform().GetLocation());
}

void ASM_PoseCheck::CheckAllOnOverlap()
{
	if (bRightOverlapped && bLeftOverlapped)
	{
		Answer_Light->SetVisibility(true);
		if (fDoorDeltaTime < 0.99f) {
			PrimaryActorTick.SetTickFunctionEnable(true);
		}
	}
}

void ASM_PoseCheck::OnRightOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bRightOverlapped = true;
	Right_Light->SetVisibility(true);
	CheckAllOnOverlap();
}

void ASM_PoseCheck::OnLeftOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bLeftOverlapped = true;
	Left_Light->SetVisibility(true);
	CheckAllOnOverlap();
}

void ASM_PoseCheck::OnRightOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bRightOverlapped = false;
	Right_Light->SetVisibility(false);
}

void ASM_PoseCheck::OnLeftOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bLeftOverlapped = false;
	Left_Light->SetVisibility(false);
}
