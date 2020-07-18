// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Boss.h"
#include "ShieldManCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "SM_Ghost_AttackObject.h"

// Sets default values
AMetaBall_Boss::AMetaBall_Boss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dynamic_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DYNAMIC_MESH"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->SetSphereRadius(80.f);

	RootComponent = Dynamic_Mesh;
	Collision->SetupAttachment(RootComponent);

	RepeatInterval = 3.0f;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	HPBarWidget->SetupAttachment(RootComponent);
	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/PlayScene/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}

	m_status = WAITING;

	MaxHP = 100.f;

	CurrentHP = 100.f;

	Opacity = 0;
}

// Called when the game starts or when spawned
void AMetaBall_Boss::BeginPlay()
{
	Super::BeginPlay();

	auto p = GetWorld()->GetPawnIterator();

	Player = Cast<AShieldManCharacter>(*p);

	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		MaterialParamName[i] = "";
		FString Name = "Ball";
		Name.AppendInt(1 + i);
		MaterialParamName[i] = FName(*Name);

	}
	float ball_size = 100.f;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				Balls_Velocity[k+3*j+9*i] = FVector(
					-ball_size+k* ball_size, 
					-ball_size + j * ball_size,
					-ball_size + i * ball_size);

				Balls_Position[i] = FVector(0, 0, 70.f);
			}
		}
	}
	
}

// Called every frame
void AMetaBall_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetRotation(DeltaTime);
	Update(DeltaTime);

	switch (m_status)
	{
	case WAITING:

		break;
	case MOVING: {
	}
			   break;
	case ATTACKING:
		GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_Boss::Attack, 1.f, false);

		m_status = WAITING;
		break;
	}
}

void AMetaBall_Boss::Update(float DeltaTime)
{
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Dynamic_Mesh->SetVectorParameterValueOnMaterials(MaterialParamName[i], Balls_Position[i]);

	}
}
void AMetaBall_Boss::SetRotation(float DeltaTime)
{
	//Anchor_Position[0].RotateAngleAxis(GetActorRotation().Yaw, FVector(0, 1, 0));
	/*for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Anchor_Position[i] = UKismetMathLibrary::GreaterGreater_VectorRotator(Anchor_Default_Position[i], GetActorRotation() * -1);
	}*/
	FRotator ToPlayerInterpRot =
		FMath::RInterpTo(prev_Rot, GetActorRotation(), DeltaTime, 3.f);
	prev_Rot += ToPlayerInterpRot - prev_Rot;

}

void AMetaBall_Boss::OnRepeatTimer()
{
	
	FVector NextLocation;
	
	m_status = MOVING;

}

void AMetaBall_Boss::Attack()
{
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Balls_Position[i] = FVector{ 0 };
	}
}
