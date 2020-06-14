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
	pit = 0;
	rol = 0;
	yaw = 0;
}

// Called when the game starts or when spawned
void ASM_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);

	for (auto FA : FoundActors)
	{
		
		control_character = Cast<AShieldManCharacter>(FA);
		//UE_LOG(LogTemp, Warning, TEXT("control_character->GetName() = %s"), FA->GetClass()->GetFName());
		//UE_LOG(LogTemp, Warning, TEXT("control_character->GetName() = %s"), *control_character->GetName());
		//Init_Camera();
	}
	//UE_LOG(LogTemp, Warning, TEXT("control_character->GetName() = %s"), *control_character->GetName());
}

// Called every frame
void ASM_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	networkManager->Send_MouseMove({ pit, yaw, rol});
	if (networkManager->RecvPacket())
	{
		updateArm(networkManager->m_recvcid);
		pit = 0.f;
		yaw = 0.f;
		rol = 0.f;
	}
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
		if (m_cid == 2 || m_cid == 0)
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

	case PPT_BODY:
	{
		control_character->AddControllerYawInput(Val);
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

	case PPT_BODY:
	{
		control_character->AddControllerPitchInput(Val);
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
		control_character->AnimInstance->AddHand_RightRot({ -Value, 0.f, 0.f });
		break;
	}

	case PPT_BODY:
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		control_character->AddMovementInput(Direction, Value);
		break;
	}

	case PPT_ARML:
	{
		control_character->AnimInstance->AddHand_LeftRot({ Value, 0.f, 0.f });
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
		control_character->AnimInstance->AddHand_RightRot({ 0.f, -Value , 0.f });
		break;
	}

	case PPT_BODY:
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		control_character->AddMovementInput(Direction, Value);
		break;
	}

	case PPT_ARML:
	{
		control_character->AnimInstance->AddHand_LeftRot({ 0.f, Value, 0.f });
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
	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	//SpringArm->SetupAttachment(control_character->GetRootComponent());
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