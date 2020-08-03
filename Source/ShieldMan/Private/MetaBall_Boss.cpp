// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Boss.h"
#include "ShieldManCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "SM_BossAttackFloor.h"
#include "SM_Ghost_AttackObject.h"
#include "MetaBall_Slime.h"
#include "MetaBall_Ghost.h"

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

	m_status = WAITING;

	v_Octahedron.emplace_back("Ball1");
	v_Octahedron.emplace_back("Ball3");
	v_Octahedron.emplace_back("Ball5");
	v_Octahedron.emplace_back("Ball6");
	v_Octahedron.emplace_back("Ball10");
	v_Octahedron.emplace_back("Ball12");
	v_Octahedron.emplace_back("Ball15");
	v_Octahedron.emplace_back("Ball19");
	v_Octahedron.emplace_back("Ball21");
	v_Octahedron.emplace_back("Ball25");
	v_Octahedron.emplace_back("Ball27");

	attack_term = 3.0f;
	avoid_term = 2.f;
	Max_Rotate = 10;
	rotate_speed = 480.f;
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
	
	num_rotate = 0;

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
	bool bResetCnt = false;

	float turn_rate = rotate_speed* DeltaTime;

	rotate_cnt += turn_rate;
	if (rotate_cnt >= 90.f) {
		float sulplus = rotate_cnt - 90.f;
		turn_rate -= sulplus;

		bResetCnt = true;
	}

	switch (Target_Row) {
	case ROTATE_X:
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				Balls_Position[row][j][k] = UKismetMathLibrary::GreaterGreater_VectorRotator(Balls_Position[row][j][k], FRotator{ 0.f,0.f, -turn_rate });
			}
		}
		break;
	case ROTATE_Y:
		for (int i = 0; i < MAX_NUM_ROW; ++i) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				Balls_Position[i][row][k] = UKismetMathLibrary::GreaterGreater_VectorRotator(Balls_Position[i][row][k], FRotator{ -turn_rate,0.f,0.f });
			}
		}
		break;
	case ROTATE_Z:
		for (int i = 0; i < MAX_NUM_ROW; ++i) {
			for (int j = 0; j < MAX_NUM_ROW; ++j) {
				Balls_Position[i][j][row] = UKismetMathLibrary::GreaterGreater_VectorRotator(Balls_Position[i][j][row], FRotator{ 0.f,-turn_rate,0.f });
			}
		}
		break;
	default:
		ULog::Invalid("Target_Row", "", LO_Viewport);
		break;
	}

	if (bResetCnt) {
		FVector temp[MAX_NUM_ROW][MAX_NUM_ROW];
		FName temp_name[MAX_NUM_ROW][MAX_NUM_ROW];
		switch (Target_Row) {
		case ROTATE_X:
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
			break;
		case ROTATE_Y:
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
			break;
		case ROTATE_Z:
			for (int i = 0; i < MAX_NUM_ROW; ++i) {
				for (int j = 0; j < MAX_NUM_ROW; ++j) {
					temp[i][j] = Balls_Position[i][j][row];
					temp_name[i][j] = MaterialParamName[i][j][row];
				}
			}
			for (int i = 0; i < MAX_NUM_ROW; ++i) {
				for (int j = 0; j < MAX_NUM_ROW; ++j) {
					Balls_Position[j][MAX_NUM_ROW - 1 - i][row] = temp[i][j];
					MaterialParamName[j][MAX_NUM_ROW - 1 - i][row] = temp_name[i][j];

				}
			}
			break;
		default:
			ULog::Invalid("Target_Row", "", LO_Viewport);
			break;
		}

		rand_target = (ROTATE_TARGET)(rand() % 3);
		rand_row = rand() % 3;
		rand_rigt = false;
		
		num_rotate++;
		if (num_rotate == Max_Rotate)
		{
			GetWorldTimerManager().SetTimer(RotateTimerHandle, this, &AMetaBall_Boss::spawn_Effect, avoid_term);
		}
		else
		{
			rotate_cnt = 0.f;
		}
	}

}

void AMetaBall_Boss::spawn_Effect()
{
	bool bAttackSuccess=false;
	FVector Pos = GetActorLocation();
	float gab = 560.f;
	float between_dis = 70.f;
	Pos.X += gab;
	Pos.Y += between_dis;
	FVector taget_Pos = Player->GetActorLocation();

	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			if (find(v_Octahedron.begin(), v_Octahedron.end(), MaterialParamName[i][0][j]) != v_Octahedron.end()) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, FVector(Pos.X - (i * gab), Pos.Y + ((MAX_NUM_ROW-j) * gab), 600), FRotator::ZeroRotator, true);
				if (!bAttackSuccess) {
					if (Pos.X - (i * gab) + (gab / 2) >= taget_Pos.X && Pos.X  - (i * gab) - (gab / 2) < taget_Pos.X) {
						if (Pos.Y + ((MAX_NUM_ROW - j) * gab) + (gab / 2) >= taget_Pos.Y && Pos.Y + ((MAX_NUM_ROW - j) * gab) - (gab / 2) < taget_Pos.Y) {
							bAttackSuccess = true;
							AMetaBall_Slime* slime =GetWorld()->SpawnActor<AMetaBall_Slime>(SlimeClass, FVector(Pos.X - (i * gab), Pos.Y + ((MAX_NUM_ROW - j) * gab), 600.f), FRotator::ZeroRotator);
							Player->DecreaseHP();
						}
					}
				}
			}
		}
	}
	if (!bAttackSuccess){
		GetWorld()->SpawnActor<AMetaBall_Ghost>(GhostClass, FVector(0.f, 40.f, 1000.f), FRotator::ZeroRotator);
	}
	
	num_rotate = 0;
	GetWorldTimerManager().SetTimer(RotateTimerHandle, this, &AMetaBall_Boss::RotateReset, attack_term);
}

void AMetaBall_Boss::RotateReset()
{

	rotate_cnt = 0.f;
}
