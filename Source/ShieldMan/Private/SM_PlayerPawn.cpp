// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_PlayerPawn.h"
#include "SMAnimInstance.h"

// Sets default values
ASM_PlayerPawn::ASM_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	control_type = PPT_BODY;
	connect_networkManager = false;
	
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	pit = 0;
	rol = 0;
	yaw = 0;


	Init_Camera();

	x_move = 0;
	y_move = 0;
	z_move = 0;

	x_camera = 0;
	y_camera = 0;
	z_camera = 0;

	control_character = NULL;
}

// Called when the game starts or when spawned
void ASM_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);

	for (auto FA : FoundActors)
	{
		//FA->GetClass()
		control_character = Cast<AShieldManCharacter>(FA);
		//UE_LOG(LogTemp, Warning, TEXT("control_character->GetName() = %s"), Cast<AShieldManCharacter>(FA)->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("control_character->GetName() = %s"), *control_character->GetName());
		//Camera = control_character->Camera;
		//SpringArm = control_character->SpringArm;
		

	}
	//UE_LOG(LogTemp, Warning, TEXT("control_character->GetName() = %s"), *control_character->GetName());
}

// Called every frame
void ASM_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	networkManager->Send_InGame(x_move, y_move, z_move, pit, yaw, rol, x_camera, y_camera, z_camera);

	if (networkManager->RecvPacket())
	{
		updateArm(networkManager->m_recvcid);
		updateBody(networkManager->m_recvcid);
		updateRotateCamera(networkManager->m_recvcid);
		updateCamera();
		pit = 0.f;
		yaw = 0.f;
		rol = 0.f;
		x_move = 0.f;
		y_move = 0.f;
		z_move = 0.f;
		x_camera = 0;
		y_camera = 0;
		z_camera = 0;
	}
}

void ASM_PlayerPawn::updateRotateCamera(int recvcid)
{
	if(recvcid == 0 || recvcid == 2)
	UE_LOG(LogTemp, Warning, TEXT("updateRotateCamera [%d] -> [%d] pitch : %f      yaw : %f"),
		recvcid, m_cid, networkManager->m_playerInfo.m_camerax, networkManager->m_playerInfo.m_cameray);

	Super::AddControllerYawInput(networkManager->m_playerInfo.m_cameray);
	Super::AddControllerPitchInput(networkManager->m_playerInfo.m_camerax);
}

void ASM_PlayerPawn::updateBody(int recvcid)
{
	switch (control_type)
	{

	case PPT_BODY:
	{
		//UE_LOG(LogTemp, Warning, TEXT("OK"));
			// find out which way is forward
		const FRotator Rotation = control_character->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		control_character->AddMovementInput(DirectionX, networkManager->m_playerInfo.m_x);
		const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		control_character->AddMovementInput(DirectionY, networkManager->m_playerInfo.m_y);
		//UE_LOG(LogTemp, Warning, TEXT("AddMovementInput x = %f"), networkManager->m_playerInfo.m_x);
		//SpringArm->AddWorldOffset({ networkManager->m_playerInfo.m_x, networkManager->m_playerInfo.m_y, networkManager->m_playerInfo.m_z });

		break;


	}

	default:
		break;
	}
}
void ASM_PlayerPawn::updateCamera()
{
	RootComponent->SetWorldLocation(control_character->GetMesh()->GetComponentLocation());
}

void ASM_PlayerPawn::SetInitialLocationCamera(FVector sprloc)
{
	RootComponent->SetWorldLocation(sprloc);
	
	//SpringArm = control_character->SpringArm;
}

void ASM_PlayerPawn::updateArm(int recvcid)
{
	switch (control_type)
	{
	case PPT_ARMR:
	{
		//UE_LOG(LogTemp, Warning, TEXT("pitch : %f      yaw : %f"), networkManager->m_playerInfo.m_pitch, networkManager->m_playerInfo.m_yaw);

		//if (m_cid == recvcid)
		//{
		//if (m_cid == 2)
			UE_LOG(LogTemp, Warning, TEXT("self [%d] -> [%d] pitch : %f      yaw : %f"), recvcid, m_cid, networkManager->m_playerInfo.m_pitch, networkManager->m_playerInfo.m_yaw);


			control_character->AnimInstance->AddHand_RightPos({ networkManager->m_playerInfo.m_pitch
			, networkManager->m_playerInfo.m_yaw, networkManager->m_playerInfo.m_roll });
		//}
		//else
		//{
			//UE_LOG(LogTemp, Warning, TEXT("other [%d] -> [%d] pitch : %f      yaw : %f"),recvcid, m_cid, networkManager->m_OtherPlayer[0].m_pitch, networkManager->m_OtherPlayer[0].m_yaw);
		//	control_character->AnimInstance->AddHand_RightPos({ networkManager->m_OtherPlayer[0].m_pitch
		//	, networkManager->m_OtherPlayer[0].m_yaw, networkManager->m_OtherPlayer[0].m_roll });
		//}


		break;
	}

	case PPT_BODY:
	{

		break;
	}

	case PPT_ARML:
	{
		control_character->AnimInstance->AddHand_LeftPos({ networkManager->m_playerInfo.m_pitch
			, networkManager->m_playerInfo.m_yaw, networkManager->m_playerInfo.m_roll });
		break;
	}

	default:
	{
		//UE_LOG
		break;
	}

	}
}

// Called to bind functionality to input
void ASM_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, control_character, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, control_character, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASM_PlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASM_PlayerPawn::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ASM_PlayerPawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ASM_PlayerPawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MouseWheel", this, &ASM_PlayerPawn::AddControllerRolInput);
}

void ASM_PlayerPawn::AddControllerYawInput(float Val)
{
	switch (control_type)
	{
	case PPT_ARMR:
	{
		yaw = Val;
		break;
	}

	case PPT_BODY:		//좌우
	{
		//y_move += Val;
		//control_character->AddControllerYawInput(Val);
		y_camera += Val;
		//Super::AddControllerYawInput(Val);
		//UE_LOG(LogTemp, Warning, TEXT(" AddControllerYawInput val : %f"), Val);
		break;
	}

	case PPT_ARML:
	{
		yaw = -Val;
		break;
	}

	default:
	{
		//UE_LOG
		break;
	}
	
	}
}

void ASM_PlayerPawn::AddControllerPitchInput(float Val)
{
	
	switch (control_type)
	{
	case PPT_ARMR:
	{
		pit += Val;
		break;
	}

	case PPT_BODY:		//앞뒤
	{

		//x_move = +Val;
		//control_character->AddControllerPitchInput(Val);
		x_camera += Val;
		//Super::AddControllerPitchInput(Val);
		//UE_LOG(LogTemp, Warning, TEXT(" AddControllerPitchInput val : %f"), Val);
		break;
	}

	case PPT_ARML:
	{
		pit += Val;
		break;
	}

	default:
	{
		//UE_LOG
		break;
	}

	}
}

void ASM_PlayerPawn::AddControllerRolInput(float Val)
{
	Val *= 2;

	switch (control_type)
	{
	case PPT_ARMR:
	{
		rol += Val;
		break;
	}

	case PPT_BODY:
	{
		//control_character->AddControllerRolInput(Val);
		//Super::AddControllerRolInput(Val);
		
		break;
	}

	case PPT_ARML:
	{
		rol += -Val;
		break;
	}

	default:
	{
		//UE_LOG
		break;
	}

	}
}

void ASM_PlayerPawn::MoveForward(float Value)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("ASM_PlayerPawn::MoveForward() GetName : %s"), *GetName());
	switch (control_type)
	{
	case PPT_ARMR:
	{
		//control_character->AnimInstance->AddHand_RightRot({ -Value, 0.f, 0.f });
		break;
	}

	case PPT_BODY:
	{
	
		x_move += Value;
		//control_character->AddMovementInput(Direction, Value);
		//UE_LOG(LogTemp, Warning, TEXT("MoveForward x_move : %f"), x_move);
		break;
	}

	case PPT_ARML:
	{
		//control_character->AnimInstance->AddHand_LeftRot({ Value, 0.f, 0.f });
		break;
	}

	default:
	{
		//UE_LOG
		break;
	}

	}
}

void ASM_PlayerPawn::MoveRight(float Value)
{
	switch (control_type)
	{
	case PPT_ARMR:
	{
		//control_character->AnimInstance->AddHand_RightRot({ 0.f, -Value , 0.f });
		break;
	}

	case PPT_BODY:
	{
		y_move += Value;
		break;
	}

	case PPT_ARML:
	{
		//control_character->AnimInstance->AddHand_LeftRot({ 0.f, Value, 0.f });
		break;
	}

	default:
	{
		//UE_LOG
		break;
	}

	}
}

void ASM_PlayerPawn::SetType(int pick)
{
	control_type = (PLAYER_POSITION_TYPE)pick;
}

void ASM_PlayerPawn::SetNetworkManager(UNetworkManager* NM)
{
	
	if (NULL != NM)
	{
		auto name2 = GetName();
		UE_LOG(LogTemp, Warning, TEXT("SetNetworkManager name2 : %s"), *GetName());
		auto p = GetWorld()->GetPawnIterator();
		
		
		for (int i = 0; i < GetWorld()->GetNumPawns(); ++i)
		{
			auto t = p->Get();
			//t->GetNetDriverNam
			//UE_LOG(LogTemp, Warning, TEXT("SetNetworkManager GetDebugName : %s"), *t->GetDebugName(this));
			auto name1 = p->Get()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("SetNetworkManager name1 : %s"), *p->Get()->GetName());
			if (name1 == name2)
			{
				networkManager = NM;
				networkManager->ConnectServer(FString("127.0.0.1"));
				networkManager->RecvPacket();
				m_cid = networkManager->m_playerInfo.m_cid;
				break;
			}
			else
			{
				++p;
			}
		}
	}
}

void ASM_PlayerPawn::Init_Camera()
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