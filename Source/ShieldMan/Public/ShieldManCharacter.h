// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BodyControl.h"
#include "RHandControl.h"
#include "LHandControl.h"
#include "ShieldMan.h"
#include "GameFramework/Character.h"
#include "ShieldManCharacter.generated.h"


UCLASS(config=Game)
class AShieldManCharacter : public ACharacter
{
	GENERATED_BODY()

private:   //private변수들
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USMAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector Hand_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FVector Hand_Left;

	ControlMode* CurControlMode;

public:		//생성자 , public 변수
	AShieldManCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected: //조작
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;

	void SetBodyControl();
	void SetRHandControl();
	void SetLHandControl();

protected:
	// APawn interface
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:	//private 함수
	void Init_Mesh();
	void Init_Camera();

public:		//public 함수

};

