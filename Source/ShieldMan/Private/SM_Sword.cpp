// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Sword.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "SM_SwordAnimInstance.h"
#include "SM_Shield.h"
#include "ShieldManCharacter.h"

// Sets default values
ASM_Sword::ASM_Sword()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.SetTickFunctionEnable(true);

	GetCapsuleComponent()->SetVisibility(false);


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT(
		"/Game/Sword/Sword.Sword"));
	if (SK_MANNEQUIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}
	else
	{
		ULog::Invalid("Sword Skeletal Not Loaded", "", LO_Viewport);
	}

	//�ִϸ��̼� ����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> SWORD_ANIM(TEXT(
		"/Game/Sword/BP_SwordAnim.BP_SwordAnim_C"));

	if (SWORD_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SWORD_ANIM.Class);
	}
	else
	{
		ULog::Invalid("Sword Anim Not Loaded", "", LO_Viewport);
	}

	On_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("COLLISION"));
	On_Collision->SetupAttachment(GetMesh(), FName(TEXT("Collision_Socket")));
	On_Collision->SetBoxExtent(FVector(70.f, 10.f, 18.f));


	static ConstructorHelpers::FClassFinder<AShieldManCharacter>CHARACTERCLASS(TEXT(
		"/Game/BP/BP_SMCharacter.BP_SMCharacter_C"
	));
	characterClass = CHARACTERCLASS.Class;

	DynamicCollision();

	EventTrigger = false;
	Waitingtime = 0.f;
	isWaiting = true;
	Rotatetime = 0.f;
	RadiusX = 100.f;
	RadiusY = 100.f;
	RotateSpeed = 50.f;
	AttackCount = 0;
	Speed = 0.f;


	GetCharacterMovement()->GravityScale = 0.f;
	GetMesh()->SetVisibility(false);
	GetMesh()->SetEnableGravity(false);
	GetCapsuleComponent()->SetEnableGravity(false);
	On_Collision->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void ASM_Sword::BeginPlay()
{
	Super::BeginPlay();

	auto p = GetWorld()->GetPawnIterator();
	Player = Cast<AShieldManCharacter>(*p);
	PrimaryActorTick.SetTickFunctionEnable(false);


}

// Called every frame
void ASM_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EventTrigger)
	{
		AnimateVariable(DeltaTime);
		Animate(DeltaTime);
	}
}

void ASM_Sword::AnimateVariable(float DeltaTime)
{
	if (isWaiting && Waitingtime > 5.f)
	{
		isWaiting = false;
		SwordAnimInstance->SetHiding(false);
		Waitingtime = 0.f;
		SwordAnimInstance->SetAttackType(rand()%3);
	}
	else if (!isWaiting)
	{
		
		isWaiting = true;
		SwordAnimInstance->SetHiding(true);
		AttackCount += 1;

		if (AttackCount >= 4)
		{
			EventTrigger = false;
			AttackCount = 0;

			FVector loc{ -5100.f, 250.f, 400.f };
			Player->TeleportTo(loc, FRotator(0.f, 0.f, 0.f));
			PrimaryActorTick.SetTickFunctionEnable(false);
		}

	}
	else
	{
		Waitingtime += DeltaTime;
	}
}

void ASM_Sword::Animate(float DeltaTime)
{
	FRotator ToPlayerInterpRot =
		FMath::RInterpTo(GetActorRotation(), (GetActorLocation() - Player->GetActorLocation()).Rotation(), DeltaTime, 0.55f);
	FVector loc{ UKismetMathLibrary::DegCos(Rotatetime) * RadiusX, UKismetMathLibrary::DegSin(Rotatetime) * RadiusY, 50.f };
	//UE_LOG(LogTemp, Log, TEXT("AttackType : %d"), SwordAnimInstance->GetAttackType());
	if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("IDLE"))
	{

		GetMesh()->SetVisibility(false);

		SetActorLocation(Player->GetActorLocation() + loc);
		Rotatetime = DeltaTime * RotateSpeed + Rotatetime;

		ToPlayerInterpRot.Pitch = 0.f;
		SetActorRotation(ToPlayerInterpRot);

		//UE_LOG(LogTemp, Log, TEXT("ToPlayerInterpRot : %s"), *ToPlayerInterpRot.ToString());

		if (Rotatetime >= 360.f)
		{
			Rotatetime = 0.f;
		}
	}

	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("TOPSLASH"))
	{

		FRotator adjust{ 0.f,45.f, 0.f };

		SetActorRelativeRotation(adjust);
		//UE_LOG(LogTemp, Log, TEXT("adjust : %s"), *adjust.ToString());
		GetMesh()->SetVisibility(true);

	}
	
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("TOPGUARD"))
	{

		//UE_LOG(LogTemp, Log, TEXT("ToPlayerInterpRot : %s"), *ToPlayerInterpRot.ToString());
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("MIDSLASH"))
	{

		GetMesh()->SetVisibility(true);
		//UE_LOG(LogTemp, Log, TEXT("ToPlayerInterpRot : %s"), *ToPlayerInterpRot.ToString());
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("MIDGUARD"))
	{

		//UE_LOG(LogTemp, Log, TEXT("ToPlayerInterpRot : %s"), *ToPlayerInterpRot.ToString());
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("BOTSLASH"))
	{

		GetMesh()->SetVisibility(true);
		//UE_LOG(LogTemp, Log, TEXT("ToPlayerInterpRot : %s"), *ToPlayerInterpRot.ToString());
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("BOTGUARD"))
	{

		//UE_LOG(LogTemp, Log, TEXT("ToPlayerInterpRot : %s"), *ToPlayerInterpRot.ToString());
	}
}

// Called to bind functionality to input
void ASM_Sword::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ASM_Sword::DynamicCollision()
{
	On_Collision->OnComponentBeginOverlap.AddDynamic(this, &ASM_Sword::OnOverlapBegin);
	On_Collision->OnComponentEndOverlap.AddDynamic(this, &ASM_Sword::OnOverlapEnd);
}


void ASM_Sword::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SwordAnimInstance->SetGuard(true);

}

void ASM_Sword::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SwordAnimInstance->SetGuard(false);
}


void ASM_Sword::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SwordAnimInstance = Cast<USM_SwordAnimInstance>(GetMesh()->GetAnimInstance());
	if (SwordAnimInstance != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("SwordAnimInstance not mapping"));

	}

}


void ASM_Sword::Begin_StageOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector loc{ -3500.f, -2500.f, -250.f };
	OtherActor->TeleportTo(loc, FRotator(0.f,0.f,0.f));
	EventTrigger = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void ASM_Sword::Begin_StageOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}



void ASM_Sword::SetCollisionFromBP(UBoxComponent* Col)
{

	Begin_Stage = Col;

	Begin_Stage->OnComponentBeginOverlap.AddDynamic(this, &ASM_Sword::Begin_StageOverlapBegin);
	Begin_Stage->OnComponentEndOverlap.AddDynamic(this, &ASM_Sword::Begin_StageOverlapEnd);
}
