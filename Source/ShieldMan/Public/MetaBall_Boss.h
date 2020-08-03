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

	STATUS m_status;

	FTimerHandle AttackedTimerHandle;

	float rotate_cnt;

	ROTATE_TARGET rand_target;
	int rand_row;
	bool rand_rigt;

	int num_rotate;

	vector<FName> v_Octahedron;

	
	FTimerHandle RotateTimerHandle;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Collision)
		USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		UParticleSystem* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		int Max_Rotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float avoid_term;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float attack_term;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float rotate_speed;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf< class AMetaBall_Ghost> GhostClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf< class AMetaBall_Slime> SlimeClass;
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

	void spawn_Effect();

	void RotateReset();
};
