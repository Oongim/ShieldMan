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

private:   //private������
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AnimInstance, meta = (AllowPrivateAccess = "true"))
	class USMAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand, meta = (AllowPrivateAccess = "true"))
	FVector Hand_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand, meta = (AllowPrivateAccess = "true"))
	FVector Hand_Left;

	ControlMode* CurControlMode;

	CharacterStatus CurrentStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand_Collision, meta = (AllowPrivateAccess = "true"))
		USphereComponent* Right_Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand_Collision, meta = (AllowPrivateAccess = "true"))
		USphereComponent* Left_Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand_Collision, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Right_Shield_Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hand_Collision, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Left_Shield_Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shield, meta = (AllowPrivateAccess = "true"))
		class ASM_Shield* Left_Shield;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shield, meta = (AllowPrivateAccess = "true"))
		class ASM_Shield* Right_Shield;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float ArmReflectPower;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
		float ShieldBoundPower;

	FTimerHandle AttackTimer;

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = AttackTime, meta = (AllowPrivateAccess = "true"))
	float AttackDelayTime;

	bool bAttackPossible;
	
	bool bDeath;

public:		//������ , public ����
	AShieldManCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Status")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Status")
		float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Status")
		FName PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		UParticleSystem* Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
		UAnimationAsset* Anim;

protected: //����
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/*�� �յ� ����*/
	void AddControllerRolInput(float Val);

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

private:	//private �Լ�
	void Init_Mesh();
	void Init_Camera();
	void Init_PhysicalAnim();

	void Set_DeathCamera();

	void Death();

public:		//public �Լ�
	UFUNCTION()
	bool CanSetShield();
	
	UFUNCTION()
	void SetShield(class ASM_Shield* NewShieldR, class ASM_Shield* NewShieldL);
	
	UFUNCTION()
	void AddForceToCharacter(FVector vDirection, float power);

	void SwitchLevel(FName LevelName);

	void SetCharacterStatus(CharacterStatus status);

	UFUNCTION()
	void OnShieldOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void ToggleAttackPossible();

	UFUNCTION()
		void DecreaseHP();
};

