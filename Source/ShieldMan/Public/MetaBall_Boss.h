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
		WAITING=0, ROTATING, ATTACKING,DEAD
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

	FVector Anchor_Position[MAX_NUM_ROW][MAX_NUM_ROW][MAX_NUM_ROW];

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float mass;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float k;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float damping;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float ShakePower;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float BoombPower;

	STATUS m_status;

	float rotate_cnt;

	ROTATE_TARGET rand_target;
	int rand_row;
	bool rand_rigt;

	int num_rotate;

	vector<FName> v_Octahedron;

	FTimerHandle RotateTimerHandle;
	FTimerHandle AttackedTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
		int HP;

public:
	

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

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> AttackedCameraShake;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake>InfiniteCameraShake;

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

	UFUNCTION(BlueprintCallable)
	void Attacked();

	void Muitiple_SpringMass_System(float timeStep);

	UFUNCTION()
	void ChangeAttackedBaseColor();

	void Dead();

	void RandShakeBall(float power);

	void BoundCheck();

	UFUNCTION(BlueprintCallable)
		void SetStatus(int status);

	UFUNCTION(BlueprintImplementableEvent)
		void StageClear();

	UFUNCTION(NetMulticast, UnReliable)
		void ServerRotateRow(int Target_Row, int row, bool bRight_Rotate, float DeltaTime);

	UFUNCTION(NetMulticast, UnReliable)
		void ServerSetStatus(int status);


	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

};
