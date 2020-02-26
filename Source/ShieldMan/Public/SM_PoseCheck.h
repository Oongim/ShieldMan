// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
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

	bool isRightOverlapped = false;
	bool isLeftOverlapped = false;

	UPROPERTY(EditAnywhere)
		UCurveFloat* DoorCurve;

	bool Open;
	bool ReadyState;
	float RotateValue;
	float CurveFloatValue;
	float TimeLineValue;
	FRotator DoorRotation;
	FTimeline MyTimeline;

public:
	UPROPERTY(BlueprintReadWrite, Category = Collision)
		UBoxComponent* Right_Collision;

	UPROPERTY(BlueprintReadWrite, Category = Collision)
		UBoxComponent* Left_Collision;

private:
	void CheckAllOnOverlap();

public:	
	// Sets default values for this actor's properties
	ASM_PoseCheck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//BP상의 CollisionBox의 레퍼런스를 받아서 세팅
	UFUNCTION(BlueprintCallable, Category = VarSetting)
		void SetCollisionFromBP(UBoxComponent* Right,UBoxComponent* Left);

	UFUNCTION()
		void OnRightOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnLeftOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnRightOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
		void OnLeftOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//UFUNCTION()
	//	void ControlDoor();

	//UFUNCTION()
	//	void ToggleDoor();

	//UFUNCTION()
	//	void SetState();
};


