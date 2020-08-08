// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "MetaBall_Ghost.generated.h"

#define MAX_NUM_BLOB 8

enum STATUS {
	WAITING, MOVING, ATTACKING
};

UCLASS()
class SHIELDMAN_API AMetaBall_Ghost : public AActor
{
	
	GENERATED_BODY()
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DynamicMesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Dynamic_Mesh;

	class AShieldManCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float ShakePower;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float min_Clamp;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float max_Clamp;

	FVector Balls_Velocity[MAX_NUM_BLOB];
	FVector Balls_Position[MAX_NUM_BLOB];
	FName MaterialParamName[MAX_NUM_BLOB];

	// INITIAL SETTINGS
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float gravity;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float mass;

	FVector Anchor_Position[MAX_NUM_BLOB];
	FVector Anchor_Default_Position[MAX_NUM_BLOB];
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float k;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float damping;


	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
		float RepeatInterval;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
		float speedPower;

	FVector EyeR_Pos;
	FVector EyeR_Default_Pos;
	FVector EyeL_Pos;
	FVector EyeL_Default_Pos;
	float EyeR_Size;
	float EyeL_Size;
	float Opacity;
	bool bEyeR_Toggle;
	bool bEyeL_Toggle;

	bool bAttacked;

	bool bAlive;

	bool bRightMove;

	STATUS m_status;
	FVector	m_destination;

	FRotator prev_Rot;

	class ASM_Ghost_AttackObject* SpawnPawn;

	FTimerHandle AttackedTimerHandle;

public:
	FTimerHandle RepeatTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Collision)
		USphereComponent* Collision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf< class ASM_Ghost_AttackObject> SpawnBulletClass;

	UPROPERTY(EditAnywhere, Category = "Shoot")
		float ShootPower;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Status")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Status")
		float CurrentHP;
public:	
	// Sets default values for this actor's properties
	AMetaBall_Ghost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Update(float DeltaTime);

	void BoundCheck(STATUS status);

	void Muitiple_SpringMass_System(float timeStep);

	UFUNCTION(BlueprintCallable, Category = floatSetting)
		void AddForceToVelocity(float power, bool bRandomShake = false);

	void Update_EyeScale(float DeltaTime);
	void SetRotation(float DeltaTime);
	virtual void OnRepeatTimer();

	void Attacked();

	virtual void MoveStart();
	void SetStatus(const STATUS status);
	void SetDestination(FVector destination);
	void SetAlive(bool alive);

	STATUS GetStatus() const;
	bool GetAlive();
	float GetOpacity();

	void Attack();


	UFUNCTION()
		void ChangeAttackedBaseColor();

	UFUNCTION(BlueprintImplementableEvent)
		void Scream();


};
