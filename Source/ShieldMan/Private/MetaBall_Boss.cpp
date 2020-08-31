// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_Boss.h"
#include "ShieldManCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "SM_BossAttackFloor.h"
#include "SM_BossGhost_AttackObject.h"
#include "MetaBall_Slime.h"
#include "MetaBall_Ghost.h"

// Sets default values
AMetaBall_Boss::AMetaBall_Boss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dynamic_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DYNAMIC_MESH"));

	RootComponent = Dynamic_Mesh;

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
	rotate_speed = 350.f;

	k = 5;
	damping = 5;
	mass = 30;
	ShakePower = 20.f;

	BoombPower = 20.f;

	HP = 27;
	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void AMetaBall_Boss::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);
	for (auto FA : FoundActors)
	{
		Player = Cast<AShieldManCharacter>(FA);
	}

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
				Anchor_Position[i][j][k] = FVector(
					-ball_size + i * ball_size,
					-ball_size + j * ball_size,
					ball_size - k * ball_size);
				Balls_Velocity[i][j][k] = FVector(0.f, 0.f, 0.f);
			}
		}
	}

	rotate_cnt = 0;

	num_rotate = 0;
	
	rand_target = 0;
	rand_row = 0;
	rand_rigt = false;
	m_status = WAITING;
}

void AMetaBall_Boss::ServerStartSetRotate_Implementation()
{
	if (ServerRow.Num()) {
		RandVal val = ServerRow[0];
		ServerRow.RemoveAt(0);
		rand_target = val.rand_target;
		rand_row = val.rand_row;
		rand_rigt = val.rand_rigt;
	}
}
void AMetaBall_Boss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
// Called every frame
void AMetaBall_Boss::Tick(float DeltaTime)
{

	//if (Role == ROLE_Authority){
	Super::Tick(DeltaTime);

	Muitiple_SpringMass_System(DeltaTime);

	Update(DeltaTime);

	switch (m_status)
	{
	case WAITING:

		break;
	case ROTATING: {
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("%d %d %d"), rand_target, rand_row, rand_rigt));
		RotateRow(static_cast<ROTATE_TARGET>(rand_target), rand_row, rand_rigt, DeltaTime);
	}
				 break;
	case DEAD:
		BoundCheck();
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

	bool bResetCnt = false;

	float turn_rate = rotate_speed * DeltaTime;
	if (rotate_cnt >= 90.f) return;
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
		//ULog::Invalid("Target_Row", "", LO_Viewport);
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
			//ULog::Invalid("Target_Row", "", LO_Viewport);
			break;
		}
		if (Role == ROLE_Authority) {
			ServerSetRotateRowVal(rand() % 3, rand() % 3, false);
		}
		if (ServerRow.Num()) {
			
			RandVal val = ServerRow[0];
			UE_LOG(LogTemp, Warning, TEXT("======%d======"), ServerRow.Num());
			UE_LOG(LogTemp, Warning, TEXT("%d %d %d"), val.rand_target, val.rand_row, val.rand_rigt);
			UE_LOG(LogTemp, Warning, TEXT("=============="));
			ServerRow.RemoveAt(0);

			rand_target = val.rand_target;
			rand_row = val.rand_row;
			rand_rigt = val.rand_rigt;
		}
		num_rotate++;
		if (num_rotate == Max_Rotate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack"));
			GetWorldTimerManager().SetTimer(RotateTimerHandle, this, &AMetaBall_Boss::spawn_Effect, avoid_term);
		}
		else
		{
			rotate_cnt = 0.f;
		}
	}
}

void AMetaBall_Boss::ServerSetRotateRowVal_Implementation(int target, int row, bool bright)
{
	ServerRow.Emplace(RandVal{ target,row,bright });

	//DOREPLIFETIME(AShieldManCharacter, RightHandPos);
}

void AMetaBall_Boss::spawn_Effect()
{
	if (Role == ROLE_Authority) {
		FVector Pos = GetActorLocation();
		float gab = 560.f;
		float between_dis = 70.f;
		Pos.X += gab;
		Pos.Y += between_dis;
		FVector taget_Pos = Player->GetActorLocation();
		bool bAttackSuccess = false;
		for (int i = 0; i < MAX_NUM_ROW; ++i) {
			for (int j = 0; j < MAX_NUM_ROW; ++j) {
				if (find(v_Octahedron.begin(), v_Octahedron.end(), MaterialParamName[i][0][j]) != v_Octahedron.end()) {

					ServerSpawn_Effect(FVector(Pos.X - (i * gab), Pos.Y + ((MAX_NUM_ROW - j) * gab), 600));

					//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, FVector(Pos.X - (i * gab), Pos.Y + ((MAX_NUM_ROW - j) * gab), 600), FRotator::ZeroRotator, true);
					if (!bAttackSuccess) {
						if (Pos.X - (i * gab) + (gab / 2) >= taget_Pos.X && Pos.X - (i * gab) - (gab / 2) < taget_Pos.X) {
							if (Pos.Y + ((MAX_NUM_ROW - j) * gab) + (gab / 2) >= taget_Pos.Y && Pos.Y + ((MAX_NUM_ROW - j) * gab) - (gab / 2) < taget_Pos.Y) {
								bAttackSuccess = true;
								AMetaBall_Slime* slime = GetWorld()->SpawnActor<AMetaBall_Slime>(SlimeClass, FVector(Pos.X - (i * gab), Pos.Y + ((MAX_NUM_ROW - j) * gab), 600.f), FRotator::ZeroRotator);
								Player->AddForceToCharacter(FVector{ 0.f,0.f,-1.f }, BoombPower);
								Player->DecreaseHP(20.f);
							}
						}
					}
				}
			}

		}
		if (!bAttackSuccess) {
			GetWorld()->SpawnActor<AMetaBall_Ghost>(GhostClass, FVector(0.f, 40.f, 1000.f), FRotator::ZeroRotator);
		}
	}
	num_rotate = 0;
	GetWorldTimerManager().SetTimer(RotateTimerHandle, this, &AMetaBall_Boss::RotateReset, attack_term);
}

void AMetaBall_Boss::ServerSpawn_Effect_Implementation(FVector pos)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, pos, FRotator::ZeroRotator, true);
}

void AMetaBall_Boss::RotateReset()
{
	rotate_cnt = 0.f;
}

void AMetaBall_Boss::Attacked()
{
	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName{ "BaseColor" }, { 0.9f, 0.1f, 0.1f });
	GetWorld()->GetTimerManager().SetTimer(AttackedTimerHandle, this, &AMetaBall_Boss::ChangeAttackedBaseColor, 0.25f, false);
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(AttackedCameraShake, 1.0f);

	RandShakeBall(ShakePower);

	HP--;
	if (HP == 0)
		Dead();
}
void AMetaBall_Boss::Dead()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(InfiniteCameraShake, 1.0f);
	m_status = DEAD;
	RandShakeBall(1000);
	k *= 3;
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				Anchor_Position[i][j][k] = FVector::ZeroVector;
			}
		}
	}
	StageClear();
}

void AMetaBall_Boss::RandShakeBall(float power)
{
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				FVector Velocity = FMath::VRand() * power;
				Balls_Velocity[i][j][k] = Velocity;
			}
		}
	}
}

void AMetaBall_Boss::ChangeAttackedBaseColor()
{
	Dynamic_Mesh->SetVectorParameterValueOnMaterials(FName{ "BaseColor" }, { 0.5, 0.1, 0.8 });
}

void AMetaBall_Boss::Muitiple_SpringMass_System(float timeStep)
{
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int w = 0; w < MAX_NUM_ROW; ++w) {
				// Mass 1 Spring Force
				FVector massSpringForce = -k * (Balls_Position[i][j][w] - Anchor_Position[i][j][w]);

				// Mass daming
				FVector massDampingForce = Balls_Velocity[i][j][w] * damping;

				// Mass net force
				FVector massForce = massSpringForce - massDampingForce + mass;

				// Mass acceleration
				FVector massAcceleration = massForce / mass;

				// Mass velocity
				Balls_Velocity[i][j][w] += massAcceleration * timeStep;

				// Mass position
				Balls_Position[i][j][w] += Balls_Velocity[i][j][w] * timeStep;
			}
		}
	}
}

void AMetaBall_Boss::BoundCheck()
{

	float min_Clamp = -300.f;
	float max_Clamp = 300.f;
	for (int i = 0; i < MAX_NUM_ROW; ++i) {
		for (int j = 0; j < MAX_NUM_ROW; ++j) {
			for (int k = 0; k < MAX_NUM_ROW; ++k) {
				Balls_Position[i][j][k] = UKismetMathLibrary::GreaterGreater_VectorRotator(Balls_Position[i][j][k], FRotator{ 0.5f,0.2f, 0.1f });

				if (Balls_Position[i][j][k].X < min_Clamp || Balls_Position[i][j][k].X > max_Clamp)
					Balls_Velocity[i][j][k].X *= -1;
				else if (Balls_Position[i][j][k].Y < min_Clamp || Balls_Position[i][j][k].Y > max_Clamp)
					Balls_Velocity[i][j][k].Y *= -1;
				else if (Balls_Position[i][j][k].Z < min_Clamp || Balls_Position[i][j][k].Z > max_Clamp)
					Balls_Velocity[i][j][k].Z *= -1;

			}
		}
	}
}

void AMetaBall_Boss::SetStatus(int status)
{
	if (Role == ROLE_Authority) {
		ServerSetStatus(status);
		ServerSetRotateRowVal(rand() % 3, rand() % 3, false);
	}
}

void AMetaBall_Boss::ServerSetStatus_Implementation(int status)
{
	m_status = static_cast<STATUS>(status);
}