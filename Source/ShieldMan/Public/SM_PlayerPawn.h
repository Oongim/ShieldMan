// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "ShieldManCharacter.h"
#include "NetworkManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "SM_PlayerPawn.generated.h"

UCLASS()
class SHIELDMAN_API ASM_PlayerPawn : public APawn
{
	GENERATED_BODY()


protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	void Init_Camera();


	float pit;
	float rol;
	float yaw;

	float x_move;
	float y_move;
	float z_move;

	float x_camera;
	float y_camera;
	float z_camera;

	bool connect_networkManager;
public:

	UNetworkManager* networkManager;

	class AShieldManCharacter* control_character;

	PLAYER_POSITION_TYPE control_type;
	// Sets default values for this pawn's properties
	ASM_PlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/*ÆÈ ¾ÕµÚ Á¶Á¾*/
	void AddControllerRolInput(float Val);

	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void SetType(int pick);

	UFUNCTION(BlueprintCallable)
		void SetNetworkManager(UNetworkManager* NM);

	void updateArm(int recvcid);
	int m_cid = 0;

	void updateBody(int recvcid);

	void updateCamera();

	UFUNCTION(BlueprintCallable)
	void SetInitialLocationCamera(FVector sprloc);

	void updateRotateCamera(int recvcid);


};
