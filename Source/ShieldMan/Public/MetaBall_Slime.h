// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "MetaBall_Slime.generated.h"

#define MAX_NUM_BLOB 6

UCLASS()
class SHIELDMAN_API AMetaBall_Slime : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DynamicMesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Dynamic_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
	float speed;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
	float min_Clamp;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
	float max_Clamp;

	FVector Balls_Velocity[MAX_NUM_BLOB];
	FVector Balls_Position[MAX_NUM_BLOB];
	FName MaterialParamName[MAX_NUM_BLOB];

	// INITIAL SETTINGS
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Slime_Option, meta = (AllowPrivateAccess = "true"))
	float gravity ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Slime_Option, meta = (AllowPrivateAccess = "true"))
	float mass ;

	FVector Anchor_Position[MAX_NUM_BLOB];
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Slime_Option, meta = (AllowPrivateAccess = "true"))
	float k ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Slime_Option, meta = (AllowPrivateAccess = "true"))
	float damping ;

public:	
	// Sets default values for this actor's properties
	AMetaBall_Slime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Update(float DeltaTime);
	void BoundCheck();

	void Muitiple_SpringMass_System(float timeStep);

};
