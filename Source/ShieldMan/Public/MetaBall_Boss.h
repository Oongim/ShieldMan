// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "MetaBall_Boss.generated.h"

#define MAX_NUM_BLOB 27

UCLASS()
class SHIELDMAN_API AMetaBall_Boss : public AActor
{

	GENERATED_BODY()
private:
	enum STATUS {
		WAITING, MOVING, ATTACKING
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DynamicMesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Dynamic_Mesh;

	class AShieldManCharacter* Player;

	FVector Balls_Velocity[MAX_NUM_BLOB];
	FVector Balls_Position[MAX_NUM_BLOB];
	FName MaterialParamName[MAX_NUM_BLOB];

	FTimerHandle RepeatTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
		float RepeatInterval;

	float Opacity;

	bool bAttacked;

	STATUS m_status;

	FRotator prev_Rot;

	FTimerHandle AttackedTimerHandle;

public:
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
	AMetaBall_Boss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Update(float DeltaTime);

	void SetRotation(float DeltaTime);

	void OnRepeatTimer();

	void Attack();

};
