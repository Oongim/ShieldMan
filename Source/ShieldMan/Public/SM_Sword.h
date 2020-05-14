// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Character.h"
#include "SM_Sword.generated.h"




UCLASS()
class SHIELDMAN_API ASM_Sword : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AnimInstance, meta = (AllowPrivateAccess = "true"))
		class USM_SwordAnimInstance* SwordAnimInstance;

	class AShieldManCharacter* Player;

	float Waitingtime;

	bool isWaiting;
	
	float Rotatetime;

public:

	UPROPERTY(EditAnywhere, Category = "Value")
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* On_Collision;

	TSubclassOf<class AShieldManCharacter> characterClass;

	UPROPERTY(BlueprintReadWrite, Category = Teleport)
		UBoxComponent* Begin_Stage;

	UFUNCTION(BlueprintCallable, Category = VS)
		void SetCollisionFromBP(UBoxComponent* Col);

	UFUNCTION()
		void Begin_StageOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Begin_StageOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, Category = "Value")
		bool EventTrigger;

	UPROPERTY(EditAnywhere, Category = "Value")
		float RotateSpeed;

	UPROPERTY(EditAnywhere, Category = "Value")
		float RadiusX;

	UPROPERTY(EditAnywhere, Category = "Value")
		float RadiusY;

	UPROPERTY(BlueprintReadWrite, Category = "Value")
		int AttackCount;

public:
	// Sets default values for this character's properties
	ASM_Sword();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AnimateVariable(float DeltaTime);

	void Animate(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DynamicCollision();
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void PostInitializeComponents() override;

};
