// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ArmPlayerPawn.h"
#include "ShieldManCharacter.h"
#include "SMAnimInstance.h"
#include "SM_GameState.h"
#include "SM_PlayerState.h"
#include "BodyControl.h"
#include "RHandControl.h"
#include "LHandControl.h"

// Sets default values
ASM_ArmPlayerPawn::ASM_ArmPlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Init_Camera();
}

// Called when the game starts or when spawned
void ASM_ArmPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);

	for (auto FA : FoundActors)
	{
		MainCharacter = Cast<AShieldManCharacter>(FA);
	}
	bStateExist = false;
	GetWorldTimerManager().SetTimer(PlayerStateTimer, this, &ASM_ArmPlayerPawn::SetPlayerState, 1.0f);
}

// Called every frame
void ASM_ArmPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bStateExist) return;
	if (nullptr == MainCharacter) {
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("MainCharacter Nullptr")));
		return;
	}

	Controller->SetControlRotation(PS->GetControllerRot());
	SetActorLocation(MainCharacter->GetActorLocation());
	//if (ControlMode->isControlMode(RHandControlMode)) {
	//	GS->RightHandPos = RightHandPos;
	//}
	//else if (ControlMode->isControlMode(LHandControlMode)) {
	//	GS->LeftHandPos = LeftHandPos;
	//}
}

// Called to bind functionality to input
void ASM_ArmPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ASM_ArmPlayerPawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ASM_ArmPlayerPawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MouseWheel", this, &ASM_ArmPlayerPawn::AddControllerRolInput);
}
void ASM_ArmPlayerPawn::AddControllerYawInput(float Val)
{
	if (bStateExist) {
		//좌 우 이동
		if (ControlMode->isControlMode(RHandControlMode)) {
			RightHandPos.Y += Val;

		}
		else if (ControlMode->isControlMode(LHandControlMode)) {
			LeftHandPos.Y -= Val;

		}
	}
}

void ASM_ArmPlayerPawn::AddControllerPitchInput(float Val)
{
	if (bStateExist) {
		//위 아래 이동
		if (ControlMode->isControlMode(RHandControlMode)) {
			RightHandPos.X += Val;
		}
		else if (ControlMode->isControlMode(LHandControlMode)) {
			LeftHandPos.X += Val;
		}
	}
}

void ASM_ArmPlayerPawn::AddControllerRolInput(float Val)
{
	if (bStateExist) {
		Val *= 2;
		//앞 뒤 이동
		if (ControlMode->isControlMode(RHandControlMode)) {
			RightHandPos.Z += Val;
		}
		else if (ControlMode->isControlMode(LHandControlMode)) {
			LeftHandPos.Z -= Val;
		}
	}
}


void ASM_ArmPlayerPawn::Init_Camera()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
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
		FVector(0.0f, 0.0f, 80.0f),
		FRotator(-5.f, 0.f, 0.f)
	);
}

void ASM_ArmPlayerPawn::SetPlayerState()
{
	if (nullptr != GetPlayerState()) {
		PS = Cast<ASM_PlayerState>(GetPlayerState());

		if ("RightArm" == PS->GetPlayerName())
			SetControlMode(RHandControlMode);
		else if ("LeftArm" == PS->GetPlayerName())
			SetControlMode(LHandControlMode);

		bStateExist = true;
	}
	else
		GetWorldTimerManager().SetTimer(PlayerStateTimer, this, &ASM_ArmPlayerPawn::SetPlayerState, 0.5f);
}


void ASM_ArmPlayerPawn::SetControlMode(EControlMode ControlType)
{
	if (LHandControlMode == ControlType)
		ControlMode = new LHandControl();

	if (RHandControlMode == ControlType)
		ControlMode = new RHandControl();
}
