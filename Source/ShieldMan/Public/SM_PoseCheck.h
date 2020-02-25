// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_PoseCheck.generated.h"

UCLASS()
class SHIELDMAN_API ASM_PoseCheck : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* PoseCheck;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* Right_Light;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* Left_Light;
public:
	UPROPERTY(BlueprintReadWrite, Category = Collision)
		UBoxComponent* Right_Collision;

	UPROPERTY(BlueprintReadWrite, Category = Collision)
		UBoxComponent* Left_Collision;

public:	
	// Sets default values for this actor's properties
	ASM_PoseCheck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
