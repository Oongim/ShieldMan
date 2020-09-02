// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ArmPlayerPawn.h"
#include "ShieldManCharacter.h"
#include "SMAnimInstance.h"
#include "SM_GameState.h"
#include "SM_PlayerState.h"
#include "BodyControl.h"
#include "RHandControl.h"
#include "LHandControl.h"
#include "SM_GameInstance.h"

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
		//SetControlMode(RHandControlMode);
	}
	bStateExist = false;

	SetPlayerState();
	//GetWorldTimerManager().SetTimer(PlayerStateTimer, this, &ASM_ArmPlayerPawn::SetPlayerState, 0.5f);;
}

// Called every frame
void ASM_ArmPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bStateExist) return;
	if (nullptr == MainCharacter) {
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("MainCharacter Nullptr")));
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);

		for (auto FA : FoundActors)
		{
			MainCharacter = Cast<AShieldManCharacter>(FA);
			//SetControlMode(RHandControlMode);
		}
		return;
	}
	PS = Cast<ASM_PlayerState>(GetPlayerState());

	USM_GameInstance* GI = Cast<USM_GameInstance>(GetGameInstance());
	//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green,
	//	FString::Printf(TEXT("%d "), GI->networkManager->m_cid));
	//if (ControlMode->isControlMode(LHandControlMode))
	if(GI->GetNetworkManager()->m_type == LHandControlMode)
	{
		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green,
		//	FString::Printf(TEXT("LControl : %s"), *PS->GetPlayerName()));
		GI->GetNetworkManager()->Send_InGame(0,0,0, pit, yaw, rol,
			0,0, 0);
		GI->GetNetworkManager()->RecvPacket();
	}
	else if (GI->GetNetworkManager()->m_type == RHandControlMode)
	{
		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green,
		//	FString::Printf(TEXT("LControl : %s"), *PS->GetPlayerName()));
		GI->GetNetworkManager()->Send_InGame(pit, yaw, rol, 0, 0, 0, 
			0,0 ,0);
		GI->GetNetworkManager()->RecvPacket();
	}
	

	

	pit = yaw = rol = 0.f;


	/*if (nullptr == AnimInstance) {
		if (GetMesh()->GetAnimInstance() != nullptr)
			AnimInstance = Cast<USMAnimInstance>(GetMesh()->GetAnimInstance());
		return;
	}*/

	if (nullptr == Controller)
	{

	}
	else
	{
		FRotator c{ GI->networkManager->m_playerInfo.cx,
			GI->networkManager->m_playerInfo.cy,0 };
		Controller->SetControlRotation(c);

		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("ASM_ArmPlayerPawn ControllerRot : %f, %f, %f"),
			//c.Pitch, c.Yaw, c.Roll));
		//GEngine->Message

		//Controller->SetControlRotation(FRotator{ c.Pitch, c.Yaw, 0 });
		SetActorLocation(MainCharacter->GetActorLocation());
		//Camera->SetWorldLocation();
		//GetWorldTransform();
		//Camera->SetWorldTransform(GetWorldTransform());
		//Camera->SetRelativeRotation(FRotator{ c.Yaw, c.Pitch, c.Roll });
	}
	
	//Camera->SetRelativeRotation(FRotator{ c.Yaw, c.Pitch, c.Roll });
	//Camera->SetWorldRotation(FRotator{ c.Yaw, c.Pitch, 0 });

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
			//RightHandPos.Y += Val;
			yaw += Val;

		}
		else if (ControlMode->isControlMode(LHandControlMode)) {
			//LeftHandPos.Y -= Val;
			yaw -= Val;

		}
	}
}

void ASM_ArmPlayerPawn::AddControllerPitchInput(float Val)
{
	if (bStateExist) {
		//위 아래 이동
		if (ControlMode->isControlMode(RHandControlMode)) {
			//RightHandPos.X += Val;
			pit += Val;
		}
		else if (ControlMode->isControlMode(LHandControlMode)) {
			//LeftHandPos.X += Val;
			pit += Val;
		}
	}
}

void ASM_ArmPlayerPawn::AddControllerRolInput(float Val)
{
	if (bStateExist) {
		Val *= 2;
		//앞 뒤 이동
		if (ControlMode->isControlMode(RHandControlMode)) {
			//RightHandPos.Z += Val;
			rol += Val;
		}
		else if (ControlMode->isControlMode(LHandControlMode)) {
			//LeftHandPos.Z -= Val;
			rol -= Val;
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
		FVector(0.0f, 0.0f, 100.0f),
		FRotator(-5.f, 0.f, 0.f)
	);
}

void ASM_ArmPlayerPawn::SetPlayerState()
{
	if (nullptr != GetPlayerState())
	{
		if (false == bStateExist)
		{
			//USM_GameInstance* GI = Cast<USM_GameInstance>(GetGameInstance());
			PS = Cast<ASM_PlayerState>(GetPlayerState());
			USM_GameInstance* GI = Cast<USM_GameInstance>(GetGameInstance());
			//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green,
			//	FString::Printf(TEXT("PS : %s"), *PS->GetPlayerName()));
			if (GI->networkManager->m_type == ARMR)
				SetControlMode(RHandControlMode);
			else if (GI->networkManager->m_type == ARML)
				SetControlMode(LHandControlMode);
			//if (FString("RightArm") == GI->name)
			//	SetControlMode(RHandControlMode);
			//else if (FString("LeftArm") == GI->name)
			//	SetControlMode(LHandControlMode);
		}
		bStateExist = true;
		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("SetPlayerState")));

	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("GetWorldTimerManager")));
		GetWorldTimerManager().SetTimer(PlayerStateTimer, this, &ASM_ArmPlayerPawn::SetPlayerState, 0.5f);
	}
}


void ASM_ArmPlayerPawn::SetControlMode(int ControlType)
{
	if (LHandControlMode == ControlType)
	{
		ControlMode = new LHandControl();
	}
	if (RHandControlMode == ControlType)
	{
		ControlMode = new RHandControl();
	}
}
