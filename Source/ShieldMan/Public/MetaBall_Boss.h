// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"

#include "GameFramework/Actor.h"
#include "MetaBall_Boss.generated.h"

#define MAX_NUM_ROW 3

UCLASS()
class SHIELDMAN_API AMetaBall_Boss : public AActor
{

	GENERATED_BODY()
private:
	enum STATUS {
		WAITING, ROTATING, ATTACKING
	};
	enum ROTATE_TARGET {
		ROTATE_X=0,
		ROTATE_Y,
		ROTATE_Z
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DynamicMesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Dynamic_Mesh;

	class AShieldManCharacter* Player;

	FVector Balls_Velocity[MAX_NUM_ROW][MAX_NUM_ROW][MAX_NUM_ROW];
	FVector Balls_Position[MAX_NUM_ROW][MAX_NUM_ROW][MAX_NUM_ROW];
	FName MaterialParamName[MAX_NUM_ROW][MAX_NUM_ROW][MAX_NUM_ROW];

	FTimerHandle RepeatTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
		float RepeatInterval;

	float Opacity;

	bool bAttacked;

	STATUS m_status;

	FRotator prev_Rot;

	FTimerHandle AttackedTimerHandle;

	float rotate_cnt;

	float rotate_speed;

	ROTATE_TARGET rand_target;
	int rand_row;
	bool rand_rigt;

	int num_rotate;

	//////////////공격 관련//////////////////////////////
	//공격관련 배열
	vector<FName> v_Octahedron;

	
	FTimerHandle RotateTimerHandle;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Collision)
		USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		UParticleSystem* Effect;

	UPROPERTY(EditAnywhere, Category = "Shoot")
		float ShootPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Status")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Status")
		float CurrentHP;


public:
	// Sets default values for this actor's properties
	AMetaBall_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Update(float DeltaTime);

	void RotateRow(ROTATE_TARGET Target_Row,int row,bool bRight_Rotate,float DeltaTime);

	void OnRepeatTimer();

	void Attack();

	void spawn_Effect();

	void RotateReset();
};
