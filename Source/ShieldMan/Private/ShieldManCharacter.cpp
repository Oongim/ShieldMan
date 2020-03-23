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
#include "SM_Shield.h"

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

	Right_Shield = CreateDefaultSubobject<ASM_Shield>(TEXT("RIGHT_SHIELD"));
	Left_Shield = CreateDefaultSubobject<ASM_Shield>(TEXT("LEFT_SHIELD"));

	MaxHP=100.f;
	CurrentHP = 80.f;
	PlayerName = TEXT("KDK");


	//Controller1 = Controller;
	Controller1 = nullptr;
	Controller2 = nullptr;
	Controller3 = nullptr;
	//UWorld* CurrentWorld = GetWorld();		//��� ���������Ʈ �˻�

	//for (const auto& Entry : FActorRange(CurrentWorld))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Actor : %s"), *Entry->GetName());
	//	TArray<UObject*> Components;
	//	Entry->GetDefaultSubobjects(Components);

	//	for (const auto& CEntry : Components)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("-- Components : %s"), *CEntry->GetName());
	//	}
	//}
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
	//�ٴڿ� ���� �� �ְ� ���� ������ �ٶ󺸴� ���� �ٲ���
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -95.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);
	
	//���̷�Ż �޽� ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT(
		"/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT(
		"/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));*/

	if (SK_MANNEQUIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	//�ִϸ��̼� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> TP_ANIM(TEXT(
		"/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));

	/*static ConstructorHelpers::FClassFinder<UAnimInstance> TP_ANIM(TEXT(
		"/Game/InfinityBladeWarriors/Animation/WarriorAnimBP.WarriorAnimBP_C"));*/

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

void AShieldManCharacter::AddForceToCharacter(FVector vDirection, float power)
{
	this->LaunchCharacter(-vDirection * power, true, false);

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
		if (CurControlMode->isControlMode(BodyControlMode)) {
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
		//�� �Ʒ� �̵�
		else if (CurControlMode->isControlMode(RHandControlMode)) {
			AnimInstance->AddHand_RightRot({ -Value, 0.f, 0.f });
		}
		else if (CurControlMode->isControlMode(LHandControlMode)) {
			AnimInstance->AddHand_LeftRot({ Value, 0.f, 0.f });
		}
		
	}
}

void AShieldManCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		if (CurControlMode->isControlMode(BodyControlMode)) {
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
		//�� �Ʒ� �̵�
		else if (CurControlMode->isControlMode(RHandControlMode)) {
			AnimInstance->AddHand_RightRot({ 0.f, -Value , 0.f});
		}
		else if (CurControlMode->isControlMode(LHandControlMode)) {
			AnimInstance->AddHand_LeftRot({ 0.f, Value, 0.f });
		}
		
	}
}

void AShieldManCharacter::PossessedBy(AController* NewController)
{
	AController* const OldController = Controller;
	
	/*Controller = NewController;

	ForceNetUpdate();*/

	if (Controller3 == nullptr)
	{
		Controller3 = NewController;
		if (Controller3->PlayerState != nullptr)
			SetPlayerState(Controller3->PlayerState);
		

		if (APlayerController* PlayerController = Cast<APlayerController>(Controller3))
		{
			SetReplicates(true);
			SetAutonomousProxy(true);

			UE_LOG(LogTemp, Log, TEXT("Controller3 : %s"), *Controller3->GetName());
		}
	}
	else if (Controller2 == nullptr)
	{
		Controller2 = NewController;
		if (Controller2->PlayerState != nullptr)
			SetPlayerState(Controller2->PlayerState);

		if (APlayerController* PlayerController = Cast<APlayerController>(Controller2))
		{
			SetReplicates(true);
			SetAutonomousProxy(true);
			UE_LOG(LogTemp, Log, TEXT("Controller2 : %s"), *Controller2->GetName());
		}
	}

	else if (Controller1 == nullptr)
	{
		Controller1 = NewController;
		if (Controller1->PlayerState != nullptr)
			SetPlayerState(Controller1->PlayerState);

		if (APlayerController* PlayerController = Cast<APlayerController>(Controller1))
		{			
			SetReplicates(true);
			SetAutonomousProxy(true);

		UE_LOG(LogTemp, Log, TEXT("Controller1 : %s"), *Controller1->GetName());
		}
	}
		
	//if (Controller3 != nullptr)
		//Controller3 = NewController;
	//Controller = NewController;
	//ForceNetUpdate();

	//if (Controller->PlayerState != nullptr)
	//{
	//	SetPlayerState(Controller->PlayerState);
	//}

	//if (Controller1 != nullptr)
	//{
	//	Controller = Controller2;

	//	ForceNetUpdate();

	//	 ReceivePossessed(Controller);

	//}

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	Camera->SetRelativeLocationAndRotation(  //ī�޶� �ʱ� ��ġ�� ���� ����
		FVector(0.0f, 0.0f, 80.0f),
		FRotator(-5.f, 0.f, 0.f)
	);
	//SpringArm->R
	//SpringArm->ReplicateSubobjects()
	//SpringArm -> bOnlyAllowAutonomousTickPose = false;
}




//void AShieldManCharacter::UnPossessed()
//{
//	//���� unpossessed
//	AController* const OldController = Controller;
//
//	ForceNetUpdate();
//
//	SetPlayerState(nullptr);
//	SetOwner(nullptr);
//	Controller = nullptr;
//
//	// Unregister input component if we created one
//	DestroyPlayerInputComponent();
//
//	// dispatch Blueprint event if necessary
//	if (OldController)
//	{
//		ReceiveUnpossessed(OldController);
//	}
//
//
//	ConsumeMovementInputVector();
//
//	////ĳ������ unpossessed
//	//if (CharacterMovement)
//	//{
//	//	CharacterMovement->ResetPredictionData_Client();
//	//	CharacterMovement->ResetPredictionData_Server();
//	//}
//
//	//// We're no longer controlled remotely, resume regular ticking of animations.
//	//if (Mesh)
//	//{
//	//	Mesh->bOnlyAllowAutonomousTickPose = false;
//	//}
//}
//void AShieldManCharacter::OnUnPossess()	//PlayerController ���
//{
//	if (GetPawn() != NULL)
//	{
//		if (Role == ROLE_Authority)
//		{
//			GetPawn()->SetReplicates(true);
//		}
//		GetPawn()->UnPossessed();
//
//		if (GetViewTarget() == GetPawn())
//		{
//			SetViewTarget(this);
//		}
//	}
//	SetPawn(NULL);
//}
//
//void AShieldManCharacter::OnPossess(APawn* PawnToPossess)	//PlayerController ���
//{
//	if (PawnToPossess != NULL &&
//		(PlayerState == NULL || !PlayerState->bOnlySpectator))
//	{
//		const bool bNewPawn = (GetPawn() != PawnToPossess);
//
//		if (GetPawn() && bNewPawn)
//		{
//			UnPossess();
//		}
//
//		if (PawnToPossess->Controller != NULL)
//		{
//			PawnToPossess->Controller->UnPossess();
//		}
//
//		PawnToPossess->PossessedBy(this);
//
//		// update rotation to match possessed pawn's rotation
//		SetControlRotation(PawnToPossess->GetActorRotation());
//
//		SetPawn(PawnToPossess);
//		check(GetPawn() != NULL);
//
//		if (GetPawn() && GetPawn()->PrimaryActorTick.bStartWithTickEnabled)
//		{
//			GetPawn()->SetActorTickEnabled(true);
//		}
//
//		INetworkPredictionInterface* NetworkPredictionInterface = GetPawn() ? Cast<INetworkPredictionInterface>(GetPawn()->GetMovementComponent()) : NULL;
//		if (NetworkPredictionInterface)
//		{
//			NetworkPredictionInterface->ResetPredictionData_Server();
//		}
//
//		AcknowledgedPawn = NULL;
//
//		// Local PCs will have the Restart() triggered right away in ClientRestart (via PawnClientRestart()), but the server should call Restart() locally for remote PCs.
//		// We're really just trying to avoid calling Restart() multiple times.
//		if (!IsLocalPlayerController())
//		{
//			GetPawn()->Restart();
//		}
//
//		ClientRestart(GetPawn());
//
//		ChangeState(NAME_Playing);
//		if (bAutoManageActiveCameraTarget)
//		{
//			AutoManageActiveCameraTarget(GetPawn());
//			ResetCameraMode();
//		}
//		// not calling UpdateNavigationComponents() anymore. The
//		// PathFollowingComponent is now observing newly possessed
//		// pawns (via OnNewPawn)
//		// need to broadcast here since we don't call Super::OnPossess
//		if (bNewPawn)
//		{
//			OnNewPawn.Broadcast(GetPawn());
//		}
//	}
//}

//void AShieldManCharacter::OnRep_Controller()
//{
//	if ((Controller1 != nullptr) && (Controller1->GetPawn() == nullptr))
//	{
//		Controller1->SetPawnFromRep(this);
//
//		APlayerController* const PC = Cast<APlayerController>(Controller1);
//		if ((PC != nullptr) && PC->bAutoManageActiveCameraTarget && (PC->PlayerCameraManager->ViewTarget.Target == Controller1))
//		{
//			PC->AutoManageActiveCameraTarget(this);
//		}
//	}
//}