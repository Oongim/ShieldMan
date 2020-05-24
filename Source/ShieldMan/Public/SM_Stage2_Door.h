// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_Stage2_Door.generated.h"

UCLASS()
class SHIELDMAN_API ASM_Stage2_Door : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Door_Frame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Door;

	bool bClosing;

	float fDoorDeltaTime = 0;
	
public:	
	// Sets default values for this actor's properties
	ASM_Stage2_Door();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = floatSetting)
	void OpenDoor();

	UFUNCTION(BlueprintCallable, Category = floatSetting)
	void CloseDoor();
};
