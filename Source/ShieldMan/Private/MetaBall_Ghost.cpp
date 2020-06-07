// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Ghost.h"
#include "ShieldManCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"

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

	MoveStart();
}

// Called every frame
void AMetaBall_Ghost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetRotation(DeltaTime);
	Update(DeltaTime);
	BoundCheck();
	Update_EyeScale(DeltaTime);

	switch (m_status)
	{
	case WAITING:
		
		break;
	case MOVING: {
		FVector dest_direct = m_destination-GetActorLocation();

		SetActorLocation(GetActorLocation()+dest_direct* DeltaTime/3);
		if ((m_destination - GetActorLocation()).Size() < 50) {
			AddForceToVelocity({ 0,0,0 }, 0.f);
			m_status = WAITING;
			MoveStart();
		}

	}
			   break;
	case ATTACKING:
		break;
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
		Anchor_Position[i] = UKismetMathLibrary::GreaterGreater_VectorRotator(Anchor_Default_Position[i], GetActorRotation()*-1);
	}
	FRotator ToPlayerInterpRot =
		FMath::RInterpTo(prev_Rot, GetActorRotation(), DeltaTime, 3.f);
	prev_Rot += ToPlayerInterpRot- prev_Rot;
	
	FVector R_Pos = UKismetMathLibrary::GreaterGreater_VectorRotator(EyeR_Default_Pos, ToPlayerInterpRot);
	FVector L_Pos = UKismetMathLibrary::GreaterGreater_VectorRotator(EyeL_Default_Pos, ToPlayerInterpRot);
	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName("EyeL_Pos"), L_Pos);
	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName("EyeR_Pos"), R_Pos);
}

void AMetaBall_Ghost::AddForceToVelocity(FVector vec, float power)
{
	prev_Rot = GetActorRotation();
	float powersize = vec.Size();
	FQuat QuatRotation = FQuat((m_destination - GetActorLocation()).Rotation());
	
	vec.Normalize();
	SetActorRotation(QuatRotation);
	FVector Velocity = /*this->GetVelocity()*/GetActorForwardVector() * power * ShakePower * 10;
	Velocity.Y /= 2;
	for (int i = 1; i < MAX_NUM_BLOB; ++i) {
		Balls_Velocity[i] = Velocity;
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

void AMetaBall_Ghost::BoundCheck()
{
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		/*if (Balls_Position[i].X < min_Clamp && Balls_Velocity[i].X < 0)
			Balls_Velocity[i].X *= -1;
		else if (Balls_Position[i].X > max_Clamp&& Balls_Velocity[i].X > 0)
			Balls_Velocity[i].X *= -1;

		if (Balls_Position[i].Y < min_Clamp && Balls_Velocity[i].Y < 0)
			Balls_Velocity[i].Y *= -1;
		else if (Balls_Position[i].Y > max_Clamp&& Balls_Velocity[i].Y > 0)
			Balls_Velocity[i].Y *= -1;*/
		if (i < 4) {
			Balls_Position[i].X = FMath::Clamp(Balls_Position[i].X, min_Clamp, max_Clamp);
			Balls_Position[i].Y = FMath::Clamp(Balls_Position[i].Y, min_Clamp, max_Clamp);
			Balls_Position[i].Z = FMath::Clamp(Balls_Position[i].Z, min_Clamp, max_Clamp);
		}
		else {
			Balls_Position[i].X = FMath::Clamp(Balls_Position[i].X, min_Clamp * 2, max_Clamp * 2);
			Balls_Position[i].Y = FMath::Clamp(Balls_Position[i].Y, min_Clamp * 2, max_Clamp * 2);
			Balls_Position[i].Z = FMath::Clamp(Balls_Position[i].Z, min_Clamp*2, max_Clamp * 2);
		}
		/*if (Balls_Position[i].Z < min_Clamp && Balls_Velocity[i].Z < 0)
			Balls_Velocity[i].Z *= -1;
		else if (Balls_Position[i].Z > max_Clamp&& Balls_Velocity[i].Z > 0)
			Balls_Velocity[i].Z *= -1;*/
	}
}

void AMetaBall_Ghost::OnRepeatTimer()
{
	if (!bAlive) return;
	FVector RunAwayVec = GetActorLocation() - Player->GetActorLocation();

	if (bAttacked)
	{
		RunAwayVec.Normalize();
		AddForceToVelocity(RunAwayVec, speedPower * 15000);
		ULog::Invalid("bAttacked", "", LO_Viewport);
	}
	else if ((RunAwayVec).Size() < 300.f)
	{
		RunAwayVec.Normalize();
		AddForceToVelocity(RunAwayVec, speedPower * 10);
		ULog::Invalid("RunAway", "", LO_Viewport);
	}
	else {
		FVector NextLocation{
					FMath::FRandRange(-3800.f, -2600.f),
					FMath::FRandRange(-630.f, 1100.f),
					FMath::FRandRange(100.f, 450.f)
		};
		m_destination = NextLocation;
		AddForceToVelocity(m_destination, speedPower * 10);
		
		m_status = MOVING;
		ULog::Invalid("NavSystem", "", LO_Viewport);
	}
}
// 공격을 받았을 때
void AMetaBall_Ghost::Attacked()
{
	if (!bAlive) return;
	bAttacked = true;
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_Ghost::OnRepeatTimer, 2.f, true);
	Health -= 20.f;

}

void AMetaBall_Ghost::MoveStart()
{
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_Ghost::OnRepeatTimer, RepeatInterval, false);
}

bool AMetaBall_Ghost::GetAlive()
{
	return bAlive;
}


//bool bSubAlpha = false;
//
//struct SDF
//{
//	float Sphere(float3 pos, float BallSize)
//	{
//		return length(pos) - BallSize;
//	}
//
//	float opUS(float d1, float d2)
//	{
//		float h = clamp(0.5 + 0.5 * (d2 - d1) / Smooth, 0.0, 1.0);
//		return lerp(d2, d1, h) - Smooth * h * (1.0 - h);
//	}
//
//	float3 RMNormal(float3 pos)
//	{
//		float2 Off = float2(0.01, 0);
//		return normalize(float3(
//			Sphere(pos + Off.xyy, 50) - Sphere(pos - Off.xyy, 50),
//			Sphere(pos + Off.yxy, 50) - Sphere(pos - Off.yxy, 50),
//			Sphere(pos + Off.yyx, 50) - Sphere(pos - Off.yyx, 50)
//		));
//	}
//	float scene(float3 pos)
//	{
//		bSubAlpha = false;
//		int numBall = 0;
//		float balls[10];
//		balls[numBall++] = Sphere(pos + Ball1, 50);
//		balls[numBall++] = Sphere(pos + Ball2, 30);
//		balls[numBall++] = Sphere(pos + Ball3, 30);
//		balls[numBall++] = Sphere(pos + Ball4, 30);
//		balls[numBall++] = Sphere(pos + Ball5, 15);
//		balls[numBall++] = Sphere(pos + Ball6, 15);
//		balls[numBall++] = Sphere(pos + Ball7, 15);
//		balls[numBall++] = Sphere(pos + Ball8, 20);
//
//		float dis = opUS(balls[0], balls[1]);
//
//		for (int i = 2; i < numBall; ++i)
//		{
//			dis = opUS(balls[i], dis);
//		}
//		float eyeL = Sphere(pos + EyeL_Pos, EyeL_Scale);
//		float eyeR = Sphere(pos + EyeR_Pos, EyeR_Scale);
//
//		dis = max(-eyeL, dis);
//		dis = max(-eyeR, dis);
//		if (max(-eyeL, dis) == -eyeL || max(-eyeR, dis) == -eyeR)
//			bSubAlpha = true;
//		return dis;
//	}
//};
//
//SDF MetaBall_SDF;
//float ray = 0.0;
//float distance;
//float4 Col = 0;
//float3 Normal = 0;
//
//for (int i = 0; i < MaxSteps; ++i)
//{
//	float3 curPos = CamPos + CamDir * ray;
//	distance = MetaBall_SDF.scene(curPos - ObjectPos);
//	if (distance <= threshold)
//	{
//		if (bSubAlpha) { 
//			Col.r = 1;
//			Col.g = 0.1;
//			Col.b = 0.1;
//			Col.a = 0.5
//		}
//		else Col = 0.9;
//		Normal = MetaBall_SDF.RMNormal(curPos - ObjectPos);
//
//		break;
//	}
//
//	ray += distance;
//}
//
//return float4(Normal, Col.a);