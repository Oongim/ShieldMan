// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShieldManCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "SMAnimInstance.h"

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
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	Init_Mesh();

	Init_Camera();

	//Init_PhysicalAnim();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	CurControlMode = new BodyControl();

	Right_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("RIGHT_COLLISION"));
	Right_Collision->SetupAttachment(GetMesh(), FName(TEXT("hand_r")));
	Right_Collision->SetSphereRadius(5.f);

	Left_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("LEFT_COLLISION"));
	Left_Collision->SetupAttachment(GetMesh(), FName(TEXT("hand_l")));
	Left_Collision->SetSphereRadius(5.f);

	Right_Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIGHT_SHIELD"));
	Left_Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LEFT_SHIELD"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_SHIELD(TEXT(
		"/Game/Import/Shield.Shield"));
	if (SK_SHIELD.Succeeded())
	{
		Right_Shield->SetSkeletalMesh(SK_SHIELD.Object);
		Left_Shield->SetSkeletalMesh(SK_SHIELD.Object);
	}

	Right_Shield->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	Left_Shield->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));

	Right_Shield->OnComponentBeginOverlap.AddDynamic(this, &AShieldManCharacter::OnOverlapBeginShield);

	MaxHP=100.f;
	CurrentHP = 80.f;
	PlayerName = TEXT("KDK");
}

void AShieldManCharacter::Init_Mesh()
{
	//�ٴڿ� ���� �� �ְ� ���� ������ �ٶ󺸴� ���� �ٲ���
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -95.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);
	
	//���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT(
		"/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_MANNEQUIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	//�ִϸ��̼� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> TP_ANIM(TEXT(
		"/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
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
	Camera->SetRelativeLocationAndRotation(  //ī�޶� �ʱ� ��ġ�� ���� ����
		FVector(0.0f, 0.0f, 80.0f),
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

void AShieldManCharacter::OnOverlapBeginShield(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	ULog::Hello(LO_Viewport);
	/*if (OtherActor->GetClass() == TSubclassOf< class ASM_ShootObjectBullet>().Get())
	{
		CurrentHP -= 10.f;
	}*/
}


//////////////////////////////////////////////////////////////////////////
// Input

void AShieldManCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<USMAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("why not --"));

	}
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
	if (CurControlMode->isControlMode(BodyControlMode)) {
		Super::AddControllerYawInput(Val);
	}
	//�� �� �̵�
	else if (CurControlMode->isControlMode(RHandControlMode)) {
		
		AnimInstance->AddHand_RightPos({ 0.f, 0.f, -Val });
	}
	else if (CurControlMode->isControlMode(LHandControlMode)) {
		AnimInstance->AddHand_LeftPos({ 0.f, 0.f, -Val });

	}
}

void AShieldManCharacter::AddControllerPitchInput(float Val)
{
	if (CurControlMode->isControlMode(BodyControlMode)) {
		Super::AddControllerPitchInput(Val);
	}
	//�� �Ʒ� �̵�
	else if (CurControlMode->isControlMode(RHandControlMode)) {
		AnimInstance->AddHand_RightPos({ -Val, 0.f, 0.f });
	}
	else if (CurControlMode->isControlMode(LHandControlMode)) {
		AnimInstance->AddHand_LeftPos({ Val, 0.f, 0.f });
	}
}


void AShieldManCharacter::AddControllerRolInput(float Val)
{
	Val *= 2;
	//�� �� �̵�
	if (CurControlMode->isControlMode(RHandControlMode)) {
	AnimInstance->AddHand_RightPos({ 0.f, -Val, 0.f });
	}
	else if (CurControlMode->isControlMode(LHandControlMode)) {
	AnimInstance->AddHand_LeftPos({ 0.f, Val, 0.f });
	}
}



void AShieldManCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShieldManCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
