// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ArmPlayerPawn.h"
#include "ShieldManCharacter.h"
#include "SMAnimInstance.h"
#include "SM_GameState.h"

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
}

// Called every frame
void ASM_ArmPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UGameplayStatics::GetGameMode(GetWorld())->GetGameState<ASM_GameState>();

	//게임모드의 로그인후 함수에서 게임스테이트에 플레이어 컨트롤러를 변수로 하나씩 저장해놓고 바꾸면 되지않을까

	/*auto p = GetWorld()->GetPlayerControllerIterator();
	p++;
	auto PC = Cast<APlayerController>(*p);
	if (PC == nullptr) { 
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("APlayerController nullptr")));
		return; 
	}*/
	SetActorLocationAndRotation(MainCharacter->GetActorLocation(), MainCharacter->GetActorRotation()/*PC->GetControlRotation()*/);
	
}

// Called to bind functionality to input
void ASM_ArmPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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