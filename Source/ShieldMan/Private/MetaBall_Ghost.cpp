// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Ghost.h"
#include "ShieldManCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "SM_Ghost_AttackObject.h"

// Sets default values
AMetaBall_Ghost::AMetaBall_Ghost()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dynamic_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DYNAMIC_MESH"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->SetSphereRadius(80.f);

	RootComponent = Dynamic_Mesh;
	Collision->SetupAttachment(RootComponent);

	ShakePower = 3;
	min_Clamp = -30.f;
	max_Clamp = 30.f;

	float gap = 20.f;
	int index = 0;
	Anchor_Default_Position[index++] = { 0,0,0 };
	Anchor_Default_Position[index++] = { 0,gap,gap };
	Anchor_Default_Position[index++] = { (float)sqrt(3.f) / 2 * gap,-gap / 2,gap };
	Anchor_Default_Position[index++] = { (float)-sqrt(3.f) / 2 * gap,-gap / 2,gap };

	k = 5;
	damping = 5;
	gravity = 1;
	mass = 10;

	RepeatInterval = 3.0f;

	speedPower = 1.f;

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

	EyeR_Pos = FVector(-30, 13, 0);
	EyeR_Default_Pos = FVector(-30, 13, 0);
	EyeL_Pos = FVector(-30, -13, 0);
	EyeL_Default_Pos = FVector(-30, -13, 0);
	EyeR_Size = rand() % 2 + 19.f;
	EyeL_Size = rand() % 2 + 19.f;
	bEyeR_Toggle = true;
	bEyeL_Toggle = false;

	bAlive = true;
	m_status = WAITING;


	MaxHP = 100.f;

	CurrentHP = 100.f;

	Opacity = 0;
}

// Called when the game starts or when spawned
void AMetaBall_Ghost::BeginPlay()
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

	for (int i = 0; i < MAX_NUM_BLOB - 4; ++i) {
		Balls_Velocity[i] = FVector(0.f, 0.f, 0.f);

		Balls_Position[i] = Anchor_Default_Position[i];

		Balls_Position[i + 3] = FVector(Anchor_Default_Position[i].X, Anchor_Default_Position[i].Y, 75.f);
	}
	Balls_Position[MAX_NUM_BLOB - 1] = FVector(0, 0, 70.f);

	bRightMove = FMath::RandBool();
	MoveStart();
}

// Called every frame
void AMetaBall_Ghost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	SetRotation(DeltaTime);
	Update(DeltaTime);
	BoundCheck(m_status);
	Update_EyeScale(DeltaTime);

	if (!bAlive) {
		Opacity -= 0.05;
		Dynamic_Mesh->SetScalarParameterValueOnMaterials(FName("Opacity"), Opacity);
		Dynamic_Mesh->SetScalarParameterValueOnMaterials(FName("EyeL_Scale"), 0);
		Dynamic_Mesh->SetScalarParameterValueOnMaterials(FName("EyeR_Scale"), 0);
		return;
	}

	if (false == Player->isDeath())
	{
		switch (m_status)
		{
		case WAITING:

			break;
		case MOVING: {
			FVector dest_direct = m_destination - GetActorLocation();

			SetActorLocation(GetActorLocation() + dest_direct * DeltaTime / 3);
			if ((m_destination - GetActorLocation()).Size() < 50) {
				AddForceToVelocity(0.f);
				m_status = ATTACKING;
			}
		}
				   break;
		case ATTACKING:
			GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_Ghost::Attack, 1.f, false);

			m_status = WAITING;
			break;
		}
	}
}

void AMetaBall_Ghost::Update(float DeltaTime)
{
	Muitiple_SpringMass_System(DeltaTime);

	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Dynamic_Mesh->SetVectorParameterValueOnMaterials(MaterialParamName[i], Balls_Position[i]);

	}
}
void AMetaBall_Ghost::SetRotation(float DeltaTime)
{
	//Anchor_Position[0].RotateAngleAxis(GetActorRotation().Yaw, FVector(0, 1, 0));
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Anchor_Position[i] = UKismetMathLibrary::GreaterGreater_VectorRotator(Anchor_Default_Position[i], GetActorRotation() * -1);
	}
	FRotator ToPlayerInterpRot =
		FMath::RInterpTo(prev_Rot, GetActorRotation(), DeltaTime, 3.f);
	prev_Rot += ToPlayerInterpRot - prev_Rot;

	FVector R_Pos = UKismetMathLibrary::GreaterGreater_VectorRotator(EyeR_Default_Pos, ToPlayerInterpRot);
	FVector L_Pos = UKismetMathLibrary::GreaterGreater_VectorRotator(EyeL_Default_Pos, ToPlayerInterpRot);
	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName("EyeL_Pos"), L_Pos);
	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName("EyeR_Pos"), R_Pos);
}

void AMetaBall_Ghost::AddForceToVelocity(float power, bool bRandomShake)
{
	prev_Rot = GetActorRotation();
	FQuat QuatRotation = FQuat((m_destination - GetActorLocation()).Rotation());

	SetActorRotation(QuatRotation);
	if (!bRandomShake) {
		FVector Velocity = GetActorForwardVector() * power * ShakePower * 10;
		Velocity.Y /= 2;
		for (int i = 1; i < MAX_NUM_BLOB; ++i) {
			Balls_Velocity[i] = Velocity;
		}

	}
	else
	{
		//ULog::Invalid("bRandomShake", "", LO_Viewport);
		FVector rand_Velocity;
		for (int i = 1; i < MAX_NUM_BLOB; ++i) {
			//rand_Velocity = FVector(FMath::FRandRange(-1.f, 1.f), FMath::FRandRange(-1.f, 1.f), FMath::FRandRange(-1.f, 1.f));
			if (i == 1)
				rand_Velocity = FVector{ 1,0,0 };
			if (i == 2)
				rand_Velocity = FVector{ 0,1,0 };
			if (i == 3)
				rand_Velocity = FVector{ 0,0,1 };
			rand_Velocity.Normalize();
			//ULog::Vector(rand_Velocity, LO_Viewport);
			rand_Velocity *= power * ShakePower * 10;
			Balls_Velocity[i] = rand_Velocity;
		}
	}
}
void AMetaBall_Ghost::Update_EyeScale(float DeltaTime)
{
	float minScale = 19.f;
	float maxScale = 21.f;
	float speed = 3.f;
	Dynamic_Mesh->SetScalarParameterValueOnMaterials(FName("EyeL_Scale"), EyeL_Size);
	Dynamic_Mesh->SetScalarParameterValueOnMaterials(FName("EyeR_Scale"), EyeR_Size);
	if (bEyeR_Toggle)
	{
		EyeR_Size += DeltaTime * speed;
		if (EyeR_Size > maxScale)
		{
			bEyeR_Toggle = false;
		}
	}
	else
	{
		EyeR_Size -= DeltaTime * speed;
		if (EyeR_Size < minScale)
		{
			bEyeR_Toggle = true;
		}
	}
	if (bEyeL_Toggle)
	{
		EyeL_Size += DeltaTime * speed;
		if (EyeL_Size > maxScale)
		{
			bEyeL_Toggle = false;
		}
	}
	else
	{
		EyeL_Size -= DeltaTime * speed;
		if (EyeL_Size < minScale)
		{
			bEyeL_Toggle = true;
		}
	}
}
void AMetaBall_Ghost::Muitiple_SpringMass_System(float timeStep)
{
	int numLeg = 3;
	for (int i = 1; i < MAX_NUM_BLOB - numLeg - 1; ++i) {
		// Mass 1 Spring Force
		FVector mass1SpringForce = -k * (Balls_Position[i] - Anchor_Position[i]);

		// Mass 2 Spring Force
		FVector mass2SpringForce = -k * (Balls_Position[i + numLeg] - Balls_Position[i]);

		// Mass 1 daming
		FVector mass1DampingForce = Balls_Velocity[i] * damping;

		// Mass 2 daming
		FVector mass2DampingForce = Balls_Velocity[i + numLeg] * damping;

		// Mass 1 net force
		float forceX = mass1SpringForce.X - mass1DampingForce.X - mass2SpringForce.X + mass2DampingForce.X;
		float forceY = mass1SpringForce.Y - mass1DampingForce.Y - mass2SpringForce.Y + mass2DampingForce.Y;
		float forceZ = mass1SpringForce.Z + mass * gravity - mass1DampingForce.Z - mass2SpringForce.Z + mass2DampingForce.Z;
		FVector mass1Force = FVector(forceX, forceY, forceZ);

		// Mass 2 net force
		forceX = mass2SpringForce.X - mass2DampingForce.X;
		forceY = mass2SpringForce.Y - mass2DampingForce.Y;
		forceZ = mass2SpringForce.Z + mass * 5 * gravity * 5 - mass2DampingForce.Z;
		FVector mass2Force = FVector(forceX, forceY, forceZ);

		// Mass 1 acceleration
		FVector mass1Acceleration = mass1Force / mass;

		// Mass 2 acceleration
		FVector mass2Acceleration = mass2Force / (mass * 5);

		// Mass1  velocity
		Balls_Velocity[i] += mass1Acceleration * timeStep;

		// Mass2  velocity
		Balls_Velocity[i + numLeg] += mass2Acceleration * timeStep;

		// Mass 1 position
		float Balls_PositionZ = FMath::Clamp(Balls_Position[i].Z + Balls_Velocity[i].Z * timeStep, 0.f, 25.f);
		Balls_Position[i].X += Balls_Velocity[i].X * timeStep;
		Balls_Position[i].Y += Balls_Velocity[i].Y * timeStep;
		Balls_Position[i].Z += Balls_Velocity[i].Z * timeStep;
		//Balls_Position[i].Z = Balls_PositionZ;


		// Mass 2 position
		Balls_Position[i + numLeg] += Balls_Velocity[i + numLeg] * timeStep;

	}
	Balls_Position[MAX_NUM_BLOB - 1] = (Balls_Position[4] + Balls_Position[5] + Balls_Position[6]) / 3;
	Balls_Position[MAX_NUM_BLOB - 1].Z -= 5;
	//ULog::Vector(Balls_Position[MAX_NUM_BLOB - 1], "Balls_Position: ", "", LO_Viewport);
}

void AMetaBall_Ghost::BoundCheck(STATUS status)
{
	switch (status)
	{
	case WAITING:
	{
		for (int i = 0; i < MAX_NUM_BLOB; ++i) {
			if (i < 4) {
				if (Balls_Position[i].X < min_Clamp || Balls_Velocity[i].X > max_Clamp)
					Balls_Velocity[i].X *= -1;
				if (Balls_Position[i].Y < min_Clamp || Balls_Velocity[i].Y > max_Clamp)
					Balls_Velocity[i].Y *= -1;
				if (Balls_Position[i].Z < min_Clamp || Balls_Velocity[i].Z > max_Clamp)
					Balls_Velocity[i].Z *= -1;
			}
			else {
				if (Balls_Position[i].X < min_Clamp * 2 || Balls_Velocity[i].X > max_Clamp * 2)
					Balls_Velocity[i].X *= -1;
				if (Balls_Position[i].Y < min_Clamp * 2 || Balls_Velocity[i].Y > max_Clamp * 2)
					Balls_Velocity[i].Y *= -1;
				if (Balls_Position[i].Z < min_Clamp * 2 || Balls_Velocity[i].Z > max_Clamp * 2)
					Balls_Velocity[i].Z *= -1;
			}
		}
	}
	break;
	case MOVING:
	{
		for (int i = 0; i < MAX_NUM_BLOB; ++i) {
			if (i < 4) {
				Balls_Position[i].X = FMath::Clamp(Balls_Position[i].X, min_Clamp, max_Clamp);
				Balls_Position[i].Y = FMath::Clamp(Balls_Position[i].Y, min_Clamp, max_Clamp);
				Balls_Position[i].Z = FMath::Clamp(Balls_Position[i].Z, min_Clamp, max_Clamp);
			}
			else {
				Balls_Position[i].X = FMath::Clamp(Balls_Position[i].X, min_Clamp * 2, max_Clamp * 2);
				Balls_Position[i].Y = FMath::Clamp(Balls_Position[i].Y, min_Clamp * 2, max_Clamp * 2);
				Balls_Position[i].Z = FMath::Clamp(Balls_Position[i].Z, min_Clamp * 2, max_Clamp * 2);
			}
		}
	}
	break;
	}

}

void AMetaBall_Ghost::OnRepeatTimer()
{
	if (!bAlive) return;

	FVector NextLocation;
	if (bRightMove)
	{
		NextLocation = {
				FMath::FRandRange(-3800.f, -2600.f),
				FMath::FRandRange(-630.f, -100.f),
				FMath::FRandRange(100.f, 450.f)
		};
		bRightMove = false;
	}
	else
	{
		NextLocation = {
				FMath::FRandRange(-3800.f, -2600.f),
				FMath::FRandRange(700.f, 1100.f),
				FMath::FRandRange(100.f, 450.f)
		};
		bRightMove = true;
	}

	m_destination = NextLocation;
	AddForceToVelocity(speedPower * 10);

	m_status = MOVING;

}
// 공격을 받았을 때
void AMetaBall_Ghost::Attacked()
{
	if (!bAlive) return;
	bAttacked = true;
	CurrentHP -= 20.f;
	AddForceToVelocity(15, true);

	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName{ "BaseColor" }, { 1.f, 0.f, 0.f });
	GetWorld()->GetTimerManager().SetTimer(AttackedTimerHandle, this, &AMetaBall_Ghost::ChangeAttackedBaseColor, 0.5f, false);
	if (CurrentHP <= 0)
		bAlive = false;
}

void AMetaBall_Ghost::MoveStart()
{ 
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_Ghost::OnRepeatTimer, RepeatInterval, false);
}

void AMetaBall_Ghost::SetAlive(bool alive)
{
	bAlive = alive;
}

bool AMetaBall_Ghost::GetAlive()
{
	return bAlive;
}

float AMetaBall_Ghost::GetOpacity()
{
	return Opacity;
}

void AMetaBall_Ghost::Attack()
{
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Balls_Position[i] = FVector{ 0 };
	}
	m_destination = Player->GetActorLocation();
	m_destination.Z += 50.f;
	AddForceToVelocity(30, true);
	SpawnPawn = GetWorld()->SpawnActor<ASM_Ghost_AttackObject>(SpawnBulletClass, GetActorLocation(), GetActorRotation());
	SpawnPawn->SetTarget(this);
	MoveStart();
}

void AMetaBall_Ghost::ChangeAttackedBaseColor()
{
	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName{ "BaseColor" }, { 0.194, 0.227, 0.675 });
}

void AMetaBall_Ghost::SetStatus(STATUS status)
{
	m_status = status;
}

STATUS AMetaBall_Ghost::GetStatus() const
{
	return m_status;
}

void AMetaBall_Ghost::SetDestination(FVector destination)
{
	m_destination = destination;
}

