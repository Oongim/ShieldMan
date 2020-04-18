// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Slime.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMetaBall_Slime::AMetaBall_Slime()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dynamic_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DYNAMIC_MESH"));
	Collision= CreateDefaultSubobject<USphereComponent>(TEXT("COLLISION"));
	Collision->SetSphereRadius(80.f);

	RootComponent = Dynamic_Mesh;
	Collision->SetupAttachment(RootComponent);

	speed = 10.f;
	min_Clamp = -40.f;
	max_Clamp = 40.f;

	float gap = 40.f;
	int index=0;
	Anchor_Default_Position[index++] = { 0,0,0 };
	Anchor_Default_Position[index++] = { 0,0,-gap };
	Anchor_Default_Position[index++] = { gap,0,0 };
	Anchor_Default_Position[index++] = { -gap,0,0 };
	Anchor_Default_Position[index++] = { 0,gap,0 };
	Anchor_Default_Position[index++] = { 0,-gap ,0};
	k = 5;
	damping = 5;
	gravity = 5;
	mass = 30;
}

// Called when the game starts or when spawned
void AMetaBall_Slime::BeginPlay()
{
	Super::BeginPlay();

	
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		MaterialParamName[i] = "";
		FString Name = "Ball";
		Name.AppendInt(1 + i);
		MaterialParamName[i]= FName(*Name);

	}

	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		Balls_Velocity[i]= FVector(0.f, 0.f, 0.f);
		Balls_Position[i] = FVector(0.f,0.f,0.f);
	}

}

// Called every frame
void AMetaBall_Slime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetRotation();
	
	Update(DeltaTime);
	
}

void AMetaBall_Slime::Update(float DeltaTime)
{
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
		Anchor_Position[i]= UKismetMathLibrary::GreaterGreater_VectorRotator(Anchor_Default_Position[i], GetActorRotation());
	}
}

void AMetaBall_Slime::BoundCheck()
{
	for (int i = 0; i < MAX_NUM_BLOB; ++i) {
		if (Balls_Position[i].X < min_Clamp && Balls_Velocity[i].X < 0)
			Balls_Velocity[i].X *= -1;
		else if(Balls_Position[i].X > max_Clamp && Balls_Velocity[i].X > 0)
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
		FVector massSpringForce = -k*(Balls_Position[i] - Anchor_Position[i]);
		
		// Mass daming
		FVector massDampingForce = Balls_Velocity[i] * damping;

		// Mass net force
		FVector massForce = massSpringForce - massDampingForce + mass ;

		// Mass acceleration
		FVector massAcceleration = massForce / mass;

		// Mass velocity
		Balls_Velocity[i] += massAcceleration * timeStep;

		// Mass position
		Balls_Position[i] += Balls_Velocity[i] * timeStep;

	}

}
void AMetaBall_Slime::AddForceToVelocity(FVector vec)
{
	for (int i = 1; i < MAX_NUM_BLOB; ++i) {
		Balls_Velocity[i] = this->GetVelocity();
	}
}



