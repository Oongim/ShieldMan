// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "MetaBall_Ghost.generated.h"

#define MAX_NUM_BLOB 8

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

	FTimerHandle RepeatTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Option, meta = (AllowPrivateAccess = "true"))
		float RepeatInterval;

	FVector EyeR_Pos;
	FVector EyeL_Pos;
	float EyeR_Size;
	float EyeL_Size;
	bool bEyeR_Toggle;
	bool bEyeL_Toggle;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Collision)
		USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
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

	void BoundCheck();

	void Muitiple_SpringMass_System(float timeStep);

	UFUNCTION(BlueprintCallable, Category = floatSetting)
		void AddForceToVelocity(FVector vec, float power);

	void Update_EyeScale(float DeltaTime);
	void SetRotation();
};
