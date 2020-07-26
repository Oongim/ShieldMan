// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Boss.h"
#include "ShieldManCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "SM_BossAttackFloor.h"
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

	m_status = WAITING;

	MaxHP = 100.f;

	CurrentHP = 100.f;

	Opacity = 0;

	v_Octahedron.emplace_back("Ball1");
	v_Octahedron.emplace_back("Ball3");
	v_Octahedron.emplace_back("Ball5");
	v_Octahedron.emplace_back("Ball6");
	v_Octahedron.emplace_back("Ball10");
	v_Octahedron.emplace_back("Ball12");
	v_Octahedron.emplace_back("Ball15");
	v_Octahedron.emplace_back("Ball19");
	v_Octahedron.emplace_back("Ball21");
	v_Octahedron.emplace_back("Ball27");

}

// Called when the game starts or when spawned
void AMetaBall_Boss::BeginPlay()
{
	Super::BeginPlay();

	auto p = GetWorld()->GetPawnIterator();

	Player = Cast<AShieldManCharacter>(*p);
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				MaterialParamName[i][j][k] = "";
				FString Name = "Ball";
				Name.AppendInt(9 * k + 3 * j + i + 1);
				MaterialParamName[i][j][k] = FName(*Name);
			}
		}
	}


	float ball_size = 150.f;
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				Balls_Position[i][j][k] = FVector(
					-ball_size + i * ball_size,
					-ball_size + j * ball_size,
					ball_size - k * ball_size);

				Balls_Velocity[i][j][k] = FVector(0.f, 0.f, 0.f);
			}
		}
	}

	rotate_cnt = 0;
	rotate_speed = 3;

	rand_target = (ROTATE_TARGET)(rand() % 3);
	rand_row = rand() % 3;
	rand_rigt = false;//static_cast<bool>(rand() % 2);
}

// Called every frame
void AMetaBall_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateRow(rand_target, rand_row, rand_rigt, DeltaTime);
	

	Update(DeltaTime);

	switch (m_status)
	{
	case WAITING:

		break;
	case ROTATING: {
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
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {

				Dynamic_Mesh->SetVectorParameterValueOnMaterials(MaterialParamName[i][j][k], Balls_Position[i][j][k]);
			}
		}
	}
}
void AMetaBall_Boss::RotateRow(ROTATE_TARGET Target_Row, int row, bool bRight_Rotate, float DeltaTime)
{
	if (rotate_cnt >= 90.f) return;
	if (bRight_Rotate) {
		rotate_speed = abs(rotate_speed);
	}
	else
	{
		rotate_speed = abs(rotate_speed)*-1;
	}


	if (Target_Row == ROTATE_X) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				Balls_Position[row][j][k] = UKismetMathLibrary::GreaterGreater_VectorRotator(Balls_Position[row][j][k], FRotator{0.f,0.f, rotate_speed });
			}
		}
	}
	if (Target_Row == ROTATE_Y) {
		for (int i = 0; i < MAX_NUM_ROW; ++i) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				Balls_Position[i][row][k] = UKismetMathLibrary::GreaterGreater_VectorRotator(Balls_Position[i][row][k], FRotator{ rotate_speed,0.f,0.f });
			}
		}
	}
	if (Target_Row == ROTATE_Z) {
		for (int i = 0; i < MAX_NUM_ROW; ++i) {
			for (int j = 0; j < MAX_NUM_ROW; ++j) {
				Balls_Position[i][j][row] = UKismetMathLibrary::GreaterGreater_VectorRotator(Balls_Position[i][j][row], FRotator{0.f,rotate_speed,0.f });
			}
		}
	}

	rotate_cnt += (abs(rotate_speed));

	if (rotate_cnt >= 90.f) {
		FVector temp[MAX_NUM_ROW][MAX_NUM_ROW];
		FName temp_name[MAX_NUM_ROW][MAX_NUM_ROW];

		if (Target_Row == ROTATE_X) {
			for (int j = 0; j < MAX_NUM_ROW; ++j) {
				for (int k = 0; k < MAX_NUM_ROW; ++k) {
					temp[j][k] = Balls_Position[row][j][k];
					temp_name[j][k] = MaterialParamName[row][j][k];
				}
			}
			
			for (int j = 0; j < MAX_NUM_ROW; ++j) {
				for (int k = 0; k < MAX_NUM_ROW; ++k) {	
					Balls_Position[row][k][MAX_NUM_ROW - 1 - j] = temp[j][k];
					MaterialParamName[row][k][MAX_NUM_ROW - 1 - j] = temp_name[j][k];
					
				}
			}
		}
		if (Target_Row == ROTATE_Y) {
			for (int i = 0; i < MAX_NUM_ROW; ++i) {
				for (int k = 0; k < MAX_NUM_ROW; ++k) {
					temp[i][k] = Balls_Position[i][row][k];
					temp_name[i][k] = MaterialParamName[i][row][k];
				}
			}

			for (int i = 0; i < MAX_NUM_ROW; ++i) {
				for (int k = 0; k < MAX_NUM_ROW; ++k) {
					Balls_Position[MAX_NUM_ROW - 1 - k][row][i] = temp[i][k];
					MaterialParamName[MAX_NUM_ROW - 1 - k][row][i] = temp_name[i][k];
				}
			}
		}
		if (Target_Row == ROTATE_Z) {
			for (int i = 0; i < MAX_NUM_ROW; ++i) {
				for (int j = 0; j < MAX_NUM_ROW; ++j) {
					temp[i][j] = Balls_Position[i][j][row];
					temp_name[i][j] = MaterialParamName[i][j][row];
				}
			}
			for (int i = 0; i < MAX_NUM_ROW; ++i) {
				for (int j = 0; j < MAX_NUM_ROW; ++j) {
					UE_LOG(LogTemp, Warning, TEXT("%s %s"), *Balls_Position[j][MAX_NUM_ROW - 1 - i][row].ToString(), *temp[i][j].ToString());
					Balls_Position[j][MAX_NUM_ROW - 1 - i][row] = temp[i][j];
					MaterialParamName[j][MAX_NUM_ROW - 1 - i][row] = temp_name[i][j];
	
				}
			}
		}

		rotate_cnt = 0.f;
		rand_target = (ROTATE_TARGET)(rand() % 3);
		rand_row = rand() % 3;
		rand_rigt = false;// static_cast<bool>(rand() % 2);

		/*for (int i = 0; i < MAX_NUM_ROW; ++i) {
			for (int j = 0; j < MAX_NUM_ROW; ++j) {
				Balls_Position[1][i][j] = FVector(0, 0, 0);
				Balls_Position[2][i][j] = FVector(0, 0, 0);
			}
		}*/
		spawn_floor();
	}
	/*FRotator ToPlayerInterpRot =
		FMath::RInterpTo(prev_Rot, GetActorRotation(), DeltaTime, 3.f);
	prev_Rot += ToPlayerInterpRot - prev_Rot;
	*/
}

void AMetaBall_Boss::OnRepeatTimer()
{

	FVector NextLocation;

	m_status = ROTATING;

}

void AMetaBall_Boss::Attack()
{
	//for (int i = 0; i < MAX_NUM_BLOB; ++i) {
	//	Balls_Position[i] = FVector{ 0 };
	//}
}

void AMetaBall_Boss::spawn_floor()
{
	for (int i = 0; i < MAX_NUM_ROW * MAX_NUM_ROW; ++i) {
		if (Floors[i] != nullptr) {
			Floors[i]->Destroy();
			Floors[i] = nullptr;
		}
	}
	FVector Pos = GetActorLocation();
	Pos.Y += 500.f;
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			if (find(v_Octahedron.begin(), v_Octahedron.end(), MaterialParamName[0][i][j]) == v_Octahedron.end()) {
				UE_LOG(LogTemp, Warning, TEXT("%s"), *MaterialParamName[0][i][j].ToString());
				Floors[i * 3 + j] = GetWorld()->SpawnActor<ASM_BossAttackFloor>(FloorClass, FVector(Pos.X + i * 200, Pos.Y + j * 200, 650), FRotator::ZeroRotator);
			}
		}
	}
	
	

}


//////////////////////////////////////////////////////
//struct SDF
//{
//	float Sphere(float3 pos, float BallSize)
//	{
//		return length(pos) - BallSize;
//	}
//
//
//	float sdTorus(float3 p, float2 t)
//	{
//		float2 q = float2(length(p.xz) - t.x, p.y);
//		return length(q) - t.y;
//	}
//	float sdOctahedron(float3 p, float s)
//	{
//		p = abs(p);
//		float m = p.x + p.y + p.z - s;
//		float3 q;
//		if (3.0 * p.x < m) q = p.xyz;
//		else if (3.0 * p.y < m) q = p.yzx;
//		else if (3.0 * p.z < m) q = p.zxy;
//		else return m * 0.57735027;
//
//		float k = clamp(0.5 * (q.z - q.y + s), 0.0, s);
//		return length(float3(q.x, q.y - s + k, q.z - k));
//	}
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
//			Sphere(pos + Off.xyy, 100) - Sphere(pos - Off.xyy, 100),
//			Sphere(pos + Off.yxy, 100) - Sphere(pos - Off.yxy, 100),
//			Sphere(pos + Off.yyx, 100) - Sphere(pos - Off.yyx, 100)
//		));
//	}
//	float scene(float3 pos)
//	{
//
//		int numBall = 0;
//		float balls[30];
//		balls[numBall++] = sdOctahedron(pos + Ball1, 100);
//		balls[numBall++] = Sphere(pos + Ball2, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball3, 100);
//		balls[numBall++] = Sphere(pos + Ball4, 100);
//		balls[numBall++] = Sphere(pos + Ball5, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball6, 100);
//		balls[numBall++] = Sphere(pos + Ball7, 100);
//		balls[numBall++] = Sphere(pos + Ball8, 100);
//		balls[numBall++] = Sphere(pos + Ball9, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball10, 100);
//		balls[numBall++] = Sphere(pos + Ball11, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball12, 100);
//		balls[numBall++] = Sphere(pos + Ball13, 100);
//		balls[numBall++] = Sphere(pos + Ball14, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball15, 100);
//		balls[numBall++] = Sphere(pos + Ball16, 100);
//		balls[numBall++] = Sphere(pos + Ball17, 100);
//		balls[numBall++] = Sphere(pos + Ball18, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball19, 100);
//		balls[numBall++] = Sphere(pos + Ball20, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball21, 100);
//		balls[numBall++] = Sphere(pos + Ball22, 100);
//		balls[numBall++] = Sphere(pos + Ball23, 100);
//		balls[numBall++] = Sphere(pos + Ball24, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball25, 100);
//		balls[numBall++] = Sphere(pos + Ball26, 100);
//		balls[numBall++] = sdOctahedron(pos + Ball27, 100);
//
//		float dis = opUS(balls[0], balls[1]);
//
//		for (int i = 2; i < numBall; ++i)
//		{
//			dis = opUS(balls[i], dis);
//		}
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
//		
//		Col = 1;
//
//		Normal = MetaBall_SDF.RMNormal(curPos - ObjectPos);
//
//		break;
//	}
//
//	ray += distance;
//}
//return float4(Normal, Col.a);