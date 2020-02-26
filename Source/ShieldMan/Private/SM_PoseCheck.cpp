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
	
	
	RootComponent = PoseCheck;
	Door->SetupAttachment(PoseCheck);
	
	Door->SetRelativeLocation(FVector(-14.f, 0.f, 0.f));

	//라이트 설정
	Right_Light->bVisible = false;
	Right_Light->LightColor = FColor(0, 0, 1, 1);
	Left_Light->bVisible = false;
	Left_Light->LightColor = FColor(1, 0, 0, 1);

	//StaticMesh 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DOOR(TEXT(
		"/Game/1Stage/PoseCheck/SM_Door.SM_Door"));
	if (SM_DOOR.Succeeded())
	{
		Door->SetStaticMesh(SM_DOOR.Object);
	}

	////타임라인 설정
	//Open = false;
	//ReadyState = true;


}

// Called when the game starts or when spawned
void ASM_PoseCheck::BeginPlay()
{
	Super::BeginPlay();
	
	//RotateValue = 1.f;

	//if (DoorCurve)
	//{
	//	FOnTimelineFloat TimelineCallback;
	//	FOnTimelineEventStatic TimelineFinishedCallback;

	//	TimelineCallback.BindUFunction(this, FName("ControlDoor"));
	//	TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

	//	MyTimeline.AddInterpFloat(DoorCurve, TimelineCallback);
	//	MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);

	//}
}

// Called every frame
void ASM_PoseCheck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MyTimeline.TickTimeline(DeltaTime);
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
	if (isRightOverlapped && isLeftOverlapped)
	{
		Door->SetRelativeLocation(FVector(-14.f, 0.f, 200.f));
	}
}

void ASM_PoseCheck::OnRightOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isRightOverlapped = true;
	Right_Light->SetVisibility(true);
	CheckAllOnOverlap();
}

void ASM_PoseCheck::OnLeftOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	isLeftOverlapped = true;
	Left_Light->SetVisibility(true);
	CheckAllOnOverlap();
}

void ASM_PoseCheck::OnRightOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isRightOverlapped = false;
	Right_Light->SetVisibility(false);
}

void ASM_PoseCheck::OnLeftOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isLeftOverlapped = false;
	Left_Light->SetVisibility(false);
}

//void ASM_PoseCheck::ControlDoor()
//{
//	TimeLineValue = MyTimeline.GetPlaybackPosition();
//	CurveFloatValue = RotateValue * DoorCurve->GetFloatValue(TimeLineValue);
//
//	FQuat NewRotation = FQuat(FRotator(0.0f, CurveFloatValue, 0.f));
//
//	Door->SetRelativeRotation(NewRotation);
//}
//
//void ASM_PoseCheck::ToggleDoor()
//{
//	if (ReadyState)
//	{
//		Open = !Open;
//
//		APawn* OurPawn = UGameplayStatics::GetPlayerPawn(this, 0);
//		FVector PawnLocation = OurPawn->GetActorLocation();
//		FVector Direction = GetActorLocation() - PawnLocation;
//		Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());
//
//		DoorRotation = Door->RelativeRotation;
//
//		if (Direction.X > 0.f)
//		{
//			RotateValue = 1.f;
//		}
//		else
//		{
//			RotateValue = -1.f;
//		}
//
//		ReadyState = false;
//		MyTimeline.PlayFromStart();
//	 }
//	else
//	{
//		ReadyState = false;
//		MyTimeline.Reverse();
//	}
//}
//
//void ASM_PoseCheck::SetState()
//{
//	ReadyState = true;
//}

