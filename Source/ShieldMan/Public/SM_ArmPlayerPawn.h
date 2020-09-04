// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Pawn.h"
#include "NetworkManager.h"
#include "SM_ArmPlayerPawn.generated.h"

UCLASS()
class SHIELDMAN_API ASM_ArmPlayerPawn : public APawn
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	class ASM_GameState* GameState;

	class AShieldManCharacter* MainCharacter;

	FTimerHandle PlayerStateTimer;

	bool bStateExist;

	class ASM_PlayerState* PS;

	class ASM_GameState* GS;

	class AGameModeBase* GM;

	class ControlMode* ControlMode;
	int CType = 0;

	FVector RightHandPos;
	FVector LeftHandPos;

public:
	// Sets default values for this pawn's properties
	ASM_ArmPlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*ÆÈ ¾ÕµÚ Á¶Á¾*/
	void AddControllerRolInput(float Val);
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Init_Camera();

	UFUNCTION()
	void SetPlayerState();

	UFUNCTION(BlueprintCallable)
	void SetControlMode(int ControlType);

	FRotator ControlRot;
	FVector ControlLoc;
	float pit = 0;
	float yaw = 0;
	float rol = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Call_Loading();
};
