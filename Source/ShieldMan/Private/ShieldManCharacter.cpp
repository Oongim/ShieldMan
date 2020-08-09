// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShieldManCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "SMAnimInstance.h"
#include "SM_Shield.h"
#include "MetaBall_Slime.h"

//////////////////////////////////////////////////////////////////////////
// AShieldManCharacter

AShieldManCharacter::AShieldManCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 100.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.2f;

	Init_Mesh();

	Init_Camera();

	//Init_PhysicalAnim();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	CurControlMode = new BodyControl();

	Right_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("RIGHT_COLLISION"));
	Right_Collision->SetupAttachment(GetMesh(), FName(TEXT("Bip001-R-Hand")));
	Right_Collision->SetSphereRadius(5.f);

	Left_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("LEFT_COLLISION"));
	Left_Collision->SetupAttachment(GetMesh(), FName(TEXT("Bip001-L-Hand")));
	Left_Collision->SetSphereRadius(5.f);

	Right_Shield_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("RIGHT_SHIELD_COLLISION"));
	Right_Shield_Collision->SetupAttachment(GetMesh(), FName(TEXT("hand_rCollision")));
	Right_Shield_Collision->SetBoxExtent(FVector(5, 2, 15));

	Left_Shield_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("LEFT_SHIELD_COLLISION"));
	Left_Shield_Collision->SetupAttachment(GetMesh(), FName(TEXT("hand_lCollision")));
	Left_Shield_Collision->SetBoxExtent(FVector(5, 2, 15));

	Right_Shield_Gaurd_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("RIGHT_SHIELD_GUARD_COLLISION"));
	Right_Shield_Gaurd_Collision->SetupAttachment(GetMesh(), FName(TEXT("hand_rCollision")));
	Right_Shield_Gaurd_Collision->SetBoxExtent(FVector(20, 2, 20));

	Left_Shield_Gaurd_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("LEFT_SHIELD_GUARD_COLLISION"));
	Left_Shield_Gaurd_Collision->SetupAttachment(GetMesh(), FName(TEXT("hand_lCollision")));
	Left_Shield_Gaurd_Collision->SetBoxExtent(FVector(20, 2, 20));

	Right_Shield = CreateDefaultSubobject<ASM_Shield>(TEXT("RIGHT_SHIELD"));
	Left_Shield = CreateDefaultSubobject<ASM_Shield>(TEXT("LEFT_SHIELD"));

	MaxHP=100.f;
	CurrentHP = 100.f;
	PlayerName = TEXT("KDK");

	bDeath = false;

	ArmReflectPower = -1000.f;
	ShieldBoundPower = 100.f;

	CurrentStatus = CharacterStatus::PossibleMove;

	AttackDelayTime = 1.f;
	bAttackPossible = false;

	Effect = CreateDefaultSubobject<UParticleSystem>(TEXT("EFFECT"));

	HPlock = false;
}

bool AShieldManCharacter::CanSetShield()
{
	return  (nullptr == Right_Shield && nullptr == Left_Shield);
}

void AShieldManCharacter::SetShield(ASM_Shield* NewShieldR, ASM_Shield* NewShieldL)
{
	if (nullptr != NewShieldR && nullptr != NewShieldL)
	{
		NewShieldR->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
		NewShieldR->SetOwner(this);
		Right_Shield = NewShieldR;

		NewShieldL->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
		NewShieldL->SetOwner(this);
		Right_Shield = NewShieldL;
	}
}

void AShieldManCharacter::Init_Mesh()
{
	//바닥에 닿을 수 있게 높이 조정과 바라보는 방향 바꿔줌
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -95.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT(
		"/Game/Import/CharacterMesh/Knight_SkeletalMesh.Knight_SkeletalMesh"));

	if (SK_MANNEQUIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}
	else
	{
		//ULog::Invalid("No Character", "", LO_Viewport);
	}
	//애니메이션 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> TP_ANIM(TEXT(
		"/Game/BP/Animation/BP_SMAnim.BP_SMAnim_C"));

	if (TP_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TP_ANIM.Class);
		
	}

}

void AShieldManCharacter::Init_Camera()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f; // The camera follows at this distance behind the character	
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create a follow camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	Camera->SetRelativeLocationAndRotation(  //카메라 초기 위치와 각도 조정
		FVector(0.0f, 0.0f, 100.0f),
		FRotator(-5.f, 0.f, 0.f)
	);
}

void AShieldManCharacter::Init_PhysicalAnim()
{
	/*PhysicalAnimation = CreateDefaultSubobject< UPhysicalAnimationComponent>(TEXT("PHYSICALANUMATION"));

	FName BoneName{ TEXT("spine_01") };
	FPhysicalAnimationData Data;
	Data.BodyName = BoneName;
	Data.bIsLocalSimulation = false;
	Data.OrientationStrength = 10000.f;
	Data.AngularVelocityStrength = 100.f;
	Data.PositionStrength = 1000.f;
	Data.VelocityStrength = 100.f;

	PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());

	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow( BoneName, Data, false);

	GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneName, true, false);*/
}

void AShieldManCharacter::Set_DeathCamera()
{
	GetController()->SetControlRotation(FRotator{FVector(GetActorForwardVector().X, GetActorForwardVector().Y,0).Rotation()});
	Camera->SetRelativeLocationAndRotation(  
		FVector(500.0f, 0.0f, 50.0f),
		FRotator(0.f, 180.f, 0.f)
	);
}

void AShieldManCharacter::Death()
{
	SwitchLevel(FName("Title"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShieldManCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	stage = GetWorld()->GetMapName();
	AnimInstance = Cast<USMAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance != nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("why not --"));

	}
	Left_Shield_Collision->OnComponentBeginOverlap.AddDynamic(this, &AShieldManCharacter::OnShieldOverlapBegin);
}

void AShieldManCharacter::SetBodyControl()
{
	if (!CurControlMode->isControlMode(BodyControlMode)) {
		delete CurControlMode;
		CurControlMode = new BodyControl();
	}
}

void AShieldManCharacter::SetRHandControl()
{
	if (!CurControlMode->isControlMode(RHandControlMode)) {
		delete CurControlMode;
		CurControlMode = new RHandControl();
	}
}

void AShieldManCharacter::SetLHandControl()
{
	if (!CurControlMode->isControlMode(LHandControlMode)) {
		delete CurControlMode;
		CurControlMode = new LHandControl();
	}
}

void AShieldManCharacter::AddForceToCharacter(FVector vDirection, float power)
{
	this->LaunchCharacter(-vDirection * power, false,false);

}

void AShieldManCharacter::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FName CurrentLevelName(*World->GetMapName());
		
		if (CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}

void AShieldManCharacter::SetCharacterStatus(CharacterStatus status)
{
	CurrentStatus = status;
}

void AShieldManCharacter::OnShieldOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAttackPossible) {
		FVector LeftArmVelocity = GetMesh()->GetPhysicsLinearVelocity(TEXT("Bip001-L-Forearm"));
		FVector RightArmVelocity = GetMesh()->GetPhysicsLinearVelocity(TEXT("Bip001-R-Forearm"));
		float power = LeftArmVelocity.Size() + RightArmVelocity.Size();
		LeftArmVelocity.Normalize();
		RightArmVelocity.Normalize();

		GetMesh()->AddImpulseToAllBodiesBelow(LeftArmVelocity * ArmReflectPower * power, TEXT("Bip001-L-Forearm"));
		GetMesh()->AddImpulseToAllBodiesBelow(RightArmVelocity * ArmReflectPower * power, TEXT("Bip001-R-Forearm"));

		FVector ImpulsePosition = (Right_Shield_Collision->GetComponentLocation() + Right_Shield_Collision->GetComponentLocation()) / 2;

		TArray<FOverlapResult> OutHits;
		FCollisionShape MyColSphere = FCollisionShape::MakeSphere(power / 10);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, ImpulsePosition,FRotator::ZeroRotator,true);
		bool isHit = GetWorld()->OverlapMultiByChannel(OutHits, ImpulsePosition,
			FQuat::Identity, ECC_GameTraceChannel7, MyColSphere);

		FColor color;
		if (isHit)
		{
			color = FColor::Green;
		}
		else
		{
			color = FColor::Red;
		}

		//DrawDebugSphere(GetWorld(), ImpulsePosition, MyColSphere.GetSphereRadius(), 30, color, true);

		if (isHit)
		{
			for (auto& Hit : OutHits)
			{
				UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>((Hit.GetActor())->GetRootComponent());
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *(Hit.GetActor())->GetName());
				if (MeshComp)
				{
					MeshComp->AddRadialImpulse(ImpulsePosition,
						power, power * ShieldBoundPower, ERadialImpulseFalloff::RIF_Constant);
					AMetaBall_Slime* slime = Cast<AMetaBall_Slime>(Hit.GetActor());
					if(slime)slime->Attacked();
				}
			}
		}

		//ULog::Number(power * ShieldBoundPower, "Power is: ", "", LO_Viewport);
		bAttackPossible = false;
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AShieldManCharacter::ToggleAttackPossible, AttackDelayTime);
	}
}

void AShieldManCharacter::ToggleAttackPossible()
{
	bAttackPossible = true;
}

void AShieldManCharacter::DecreaseHP(float val)
{
	if (false == HPlock)
	{
		CurrentHP -= val;
		if (CurrentHP <= 0)
		{
			ChangeDeath();
		}
	}
}

void AShieldManCharacter::ChangeDeath()
{
	GetMesh()->PlayAnimation(Anim, false);
	//ULog::Number(Anim->GetMaxCurrentTime(), "Time: ", "", LO_Viewport);
	Set_DeathCamera();
	bDeath = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AShieldManCharacter::Death, Anim->GetMaxCurrentTime());
}

void AShieldManCharacter::ChangeMagmaDeath()
{
	
	GetController()->SetControlRotation(FRotator{ FVector(GetActorForwardVector().X, GetActorForwardVector().Y,0).Rotation() });
	Camera->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, 500.0f),
		FRotator(-60.f, 0.f, 0.f)
	);
	bDeath = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AShieldManCharacter::Death, Anim->GetMaxCurrentTime());
}


bool AShieldManCharacter::isDeath()
{
	return bDeath;
}

void AShieldManCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("SetBodyControl", IE_Pressed, this, &AShieldManCharacter::SetBodyControl);
	PlayerInputComponent->BindAction("SetRHandControl", IE_Released, this, &AShieldManCharacter::SetRHandControl);
	PlayerInputComponent->BindAction("SetLHandControl", IE_Released, this, &AShieldManCharacter::SetLHandControl);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShieldManCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShieldManCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AShieldManCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AShieldManCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MouseWheel", this, &AShieldManCharacter::AddControllerRolInput);

}

void AShieldManCharacter::AddControllerYawInput(float Val)
{
	if (bDeath) return;
	if (CurControlMode->isControlMode(BodyControlMode)) {
		//if(CurrentStatus== CharacterStatus::PossibleMove)
			Super::AddControllerYawInput(Val);
	}
	//좌 우 이동
	else if (CurControlMode->isControlMode(RHandControlMode)) {
		AnimInstance->AddHand_RightPos({ 0.f,  Val,0.f });
	}
	else if (CurControlMode->isControlMode(LHandControlMode)) {
		AnimInstance->AddHand_LeftPos({ 0.f,  -Val ,0.f });

	}
}

void AShieldManCharacter::AddControllerPitchInput(float Val)
{
	if (bDeath) return;
	if (CurControlMode->isControlMode(BodyControlMode)) {
		//if (CurrentStatus == CharacterStatus::PossibleMove)
			Super::AddControllerPitchInput(Val);
	}
	//위 아래 이동
	else if (CurControlMode->isControlMode(RHandControlMode)) {
		AnimInstance->AddHand_RightPos({ Val, 0.f, 0.f });
	}
	else if (CurControlMode->isControlMode(LHandControlMode)) {
		AnimInstance->AddHand_LeftPos({ Val, 0.f, 0.f });
	}
}

void AShieldManCharacter::AddControllerRolInput(float Val)
{
	if (bDeath) return;
	Val *= 2;
	//앞 뒤 이동
	if (CurControlMode->isControlMode(RHandControlMode)) {
		AnimInstance->AddHand_RightPos({ 0.f, 0.f, Val });
	}
	else if (CurControlMode->isControlMode(LHandControlMode)) {
		AnimInstance->AddHand_LeftPos({ 0.f, 0.f, -Val });
	}
}

void AShieldManCharacter::MoveForward(float Value)
{
	if (bDeath) return;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
		//위 아래 이동
		if (CurControlMode->isControlMode(RHandControlMode)) {
			AnimInstance->AddHand_RightRot({ -Value, 0.f, 0.f });
		}
		else if (CurControlMode->isControlMode(LHandControlMode)) {
			AnimInstance->AddHand_LeftRot({ Value, 0.f, 0.f });
		}
		
	}
}

void AShieldManCharacter::MoveRight(float Value)
{
	if (bDeath) return;
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
		//위 아래 이동
		if (CurControlMode->isControlMode(RHandControlMode)) {
			AnimInstance->AddHand_RightRot({ 0.f, -Value , 0.f});
		}
		else if (CurControlMode->isControlMode(LHandControlMode)) {
			AnimInstance->AddHand_LeftRot({ 0.f, Value, 0.f });
		}
		
	}
}

