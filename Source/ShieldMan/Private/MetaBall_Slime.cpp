// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Slime.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "MetaBall_Slime.h"
#include "ShieldManCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values
AMetaBall_Slime::AMetaBall_Slime()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dynamic_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DYNAMIC_MESH"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->SetSphereRadius(80.f);

	RootComponent = Dynamic_Mesh;
	Collision->SetupAttachment(RootComponent);

	ShakePower = 3;
	min_Clamp = -40.f;
	max_Clamp = 40.f;

	float gap = 40.f;
	int index = 0;
	Anchor_Default_Position[index++] = { 0,0,0 };
	Anchor_Default_Position[index++] = { 0,0,-gap };
	Anchor_Default_Position[index++] = { gap,0,0 };
	Anchor_Default_Position[index++] = { -gap,0,0 };
	Anchor_Default_Position[index++] = { 0,gap,0 };
	Anchor_Default_Position[index++] = { 0,-gap ,0 };
	k = 5;
	damping = 5;
	gravity = 5;
	mass = 30;



	Health = 100.0f;

	speedPower = 1.f;

	RepeatInterval = 3.0f;

	Ball_Size = 50.f;

	bAttacked = false;

	bAlive = true;

	ShakeClamp = 500.f;

	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void AMetaBall_Slime::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);

	for (auto FA : FoundActors)
	{
		Player = Cast<AShieldManCharacter>(FA);
	}

	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		MaterialParamName[i] = "";
		FString Name = "Ball";
		Name.AppendInt(1 + i);
		MaterialParamName[i] = FName(*Name);

	}

	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Balls_Velocity[i] = FVector(0.f, 0.f, 0.f);
		Balls_Position[i] = FVector(0.f, 0.f, 0.f);
	}
}

// Called every frame
void AMetaBall_Slime::Tick(float DeltaTime)
{
	if (HasAuthority()) {
		if (!bAlive) return;

		Super::Tick(DeltaTime);

		ServerUpdateMetaBall(DeltaTime);
	}
}

void AMetaBall_Slime::Update(float DeltaTime)
{
	SetRotation();
	Muitiple_SpringMass_System(DeltaTime);
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Dynamic_Mesh->SetVectorParameterValueOnMaterials(MaterialParamName[i], Balls_Position[i]);
	}
	BoundCheck();
}

void AMetaBall_Slime::SetRotation()
{
	//Anchor_Position[0].RotateAngleAxis(GetActorRotation().Yaw, FVector(0, 1, 0));
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Anchor_Position[i] = UKismetMathLibrary::GreaterGreater_VectorRotator(Anchor_Default_Position[i], GetActorRotation() * -1);
	}
}

void AMetaBall_Slime::BoundCheck()
{
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		if (Balls_Position[i].X < min_Clamp && Balls_Velocity[i].X < 0)
			Balls_Velocity[i].X *= -1;
		else if (Balls_Position[i].X > max_Clamp && Balls_Velocity[i].X > 0)
			Balls_Velocity[i].X *= -1;

		if (Balls_Position[i].Y < min_Clamp && Balls_Velocity[i].Y < 0)
			Balls_Velocity[i].Y *= -1;
		else if (Balls_Position[i].Y > max_Clamp && Balls_Velocity[i].Y > 0)
			Balls_Velocity[i].Y *= -1;

		if (Balls_Position[i].Z < min_Clamp && Balls_Velocity[i].Z < 0)
			Balls_Velocity[i].Z *= -1;
		else if (Balls_Position[i].Z > max_Clamp && Balls_Velocity[i].Z > 0)
			Balls_Velocity[i].Z *= -1;
	}
}

void AMetaBall_Slime::Muitiple_SpringMass_System(float timeStep)
{
	for (int i = 1; i < MAX_NUM_BLOB; ++i) {
		// Mass 1 Spring Force
		FVector massSpringForce = -k * (Balls_Position[i] - Anchor_Position[i]);

		// Mass daming
		FVector massDampingForce = Balls_Velocity[i] * damping;

		// Mass net force
		FVector massForce = massSpringForce - massDampingForce + mass;

		// Mass acceleration
		FVector massAcceleration = massForce / mass;

		// Mass velocity
		Balls_Velocity[i] += massAcceleration * timeStep;

		// Mass position
		Balls_Position[i] += Balls_Velocity[i] * timeStep;

	}
}

void AMetaBall_Slime::ServerUpdateMetaBall_Implementation(float timeStep)
{
	Update(timeStep);
}

void AMetaBall_Slime::AddForceToVelocity(FVector vec, float power)
{

	float powersize = vec.Size();
	vec.Normalize();
	FQuat QuatRotation = FQuat((vec - GetActorForwardVector()).Rotation());

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);

	Dynamic_Mesh->AddForce(vec * power * powersize, NAME_None, true);

	FVector Velocity = this->GetVelocity() * ShakePower * 10;
	Velocity.Y /= 2;

	Velocity.X = FMath::Clamp(Velocity.X, 0.f, ShakeClamp);
	Velocity.Y = FMath::Clamp(Velocity.Y, 0.f, ShakeClamp);
	Velocity.Z = FMath::Clamp(Velocity.Z, 0.f, ShakeClamp);
	for (int i = 1; i < MAX_NUM_BLOB; ++i) {
		Balls_Velocity[i] = Velocity;
	}
}

void AMetaBall_Slime::OnRepeatTimer()
{
	if (!bAlive) return;
	FVector RunAwayVec = GetActorLocation() - Player->GetActorLocation();

	if (bAttacked)
	{
		RunAwayVec.Normalize();
		ServerAddForceToVelocity(RunAwayVec, speedPower * 1500);
	}
	else if ((RunAwayVec).Size() < 300.f)
	{
		RunAwayVec.Normalize();
		ServerAddForceToVelocity(RunAwayVec, speedPower * 1500);
	}
	else {
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (nullptr == NavSystem) return;
		FNavLocation NextLocation;
		if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, NextLocation))
		{
			ServerAddForceToVelocity(NextLocation.Location, speedPower * 30);
		}
	}
}

// 공격을 받았을 때
void AMetaBall_Slime::Attacked()
{
	if (!bAlive) return;
	bAttacked = true;
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_Slime::OnRepeatTimer, 2.f, true);
	Health -= 20.f;
	Ball_Size -= 10;		//구의 크기를 줄인다.
	if (HasAuthority()) {
		ServerUpdateBallSize(Ball_Size);
	}
	//Dynamic_Mesh->SetScalarParameterValueOnMaterials(TEXT("BallSize"), Ball_Size);
	
	if (Ball_Size < 20.f) {
		Ball_Size = 1.f;
		bAlive = false;
	}
}
void AMetaBall_Slime::ServerUpdateBallSize_Implementation(float size)
{
	Dynamic_Mesh->SetScalarParameterValueOnMaterials(TEXT("BallSize"), size);
}
void AMetaBall_Slime::MoveStart()
{
	if (HasAuthority()) {
		GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_Slime::OnRepeatTimer, RepeatInterval, true);
	}
}

bool AMetaBall_Slime::GetAlive()
{
	return bAlive;
}

void AMetaBall_Slime::setSpeedPower(float power)
{
	speedPower = power;
}

void AMetaBall_Slime::ServerAddForceToVelocity_Implementation(FVector vec, float power)
{
	AddForceToVelocity(vec, power);
}



