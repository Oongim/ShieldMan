// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_Sword.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/GameInstance.h"

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
		//ULog::Invalid("Sword Skeletal Not Loaded", "", LO_Viewport);
	}

	//애니메이션 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> SWORD_ANIM(TEXT(
		"/Game/Sword/BP_SwordAnim.BP_SwordAnim_C"));

	if (SWORD_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(SWORD_ANIM.Class);
	}
	else
	{
		//ULog::Invalid("Sword Anim Not Loaded", "", LO_Viewport);
	}

	On_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("COLLISION"));
	On_Collision->SetupAttachment(GetMesh(), FName(TEXT("Collision_Socket")));
	On_Collision->SetBoxExtent(FVector(70.f, 10.f, 18.f));
	On_Collision->BodyInstance.SetCollisionProfileName(TEXT("Sword"));
	On_Collision->SetSimulatePhysics(true);
	On_Collision->SetNotifyRigidBodyCollision(true);

	static ConstructorHelpers::FClassFinder<AShieldManCharacter>CHARACTERCLASS(TEXT(
		"/Game/BP/BP_SMCharacter.BP_SMCharacter_C"
	));
	characterClass = CHARACTERCLASS.Class;

	DynamicCollision();

	isHit = false;
	EventTrigger = false;
	Waitingtime = 0.f;
	isWaiting = true;
	Rotatetime = 0.f;
	RadiusX = 250.f;
	RadiusY = 250.f;
	RotateSpeed = 50.f;
	AttackCount = 0;
	Speed = 0.f;
	GuardCount = 0;
	MaxGuardCount = 4;

	attacktype = 0;

	GetCharacterMovement()->GravityScale = 0.f;
	GetMesh()->SetVisibility(true);
	GetMesh()->SetEnableGravity(false);
	GetCapsuleComponent()->SetEnableGravity(false);
	On_Collision->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void ASM_Sword::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);
	for (auto FA : FoundActors)
	{
		Player = Cast<AShieldManCharacter>(FA);
	}
	
	//auto p = GetWorld()->GetPawnIterator();
	//Player = Cast<AShieldManCharacter>(*p);
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
		attacktype = (attacktype + 1) % 3;
		SwordAnimInstance->SetAttackType(attacktype);
		if (GuardCount >= MaxGuardCount)
		{
			EventTrigger = false;
			GuardCount = 0;
			Player->SetActorLocation(Endto);
			PrimaryActorTick.SetTickFunctionEnable(false);
			Destroy();
		}
	}
	else if (!isWaiting)
	{
		isWaiting = true;
		SwordAnimInstance->SetHiding(true);
	}
	else
	{
		Waitingtime += DeltaTime;
	}

	
}

void ASM_Sword::Animate(float DeltaTime)
{

	FRotator ToPlayerInterpRot =
		FMath::RInterpTo(GetActorRotation(), (GetActorLocation() - Player->GetActorLocation()).Rotation(), DeltaTime, 0.60f);
	FVector loc{ UKismetMathLibrary::DegCos(Rotatetime) * RadiusX, UKismetMathLibrary::DegSin(Rotatetime) * RadiusY, 0.f };
	//UE_LOG(LogTemp, Log, TEXT("AttackType : %d"), SwordAnimInstance->GetAttackType());
	if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("IDLE"))
	{

		//GetMesh()->SetVisibility(true);
		GetMesh()->SetVisibility(false);
		SwordAnimInstance->SetGuard(false);
		isHit = false;

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
		GetMesh()->SetVisibility(true);
	}
	
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("TOPGUARD"))
	{
		
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("MIDSLASH"))
	{
		GetMesh()->SetVisibility(true);
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("MIDGUARD"))
	{
		
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("BOTSLASH"))
	{
		GetMesh()->SetVisibility(true);
	}
	else if (SwordAnimInstance->GetCurrentStateName(0).ToString() == FString("BOTGUARD"))
	{
		
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

void ASM_Sword::ServerOverlapBegin_Implementation(UPrimitiveComponent* OtherComp)
{
	if (!isHit)
	{
		FString OtherCompName = OtherComp->GetName();
		//UE_LOG(LogTemp, Log, TEXT("OverlappedComp : %s"), *OtherComp->GetName());			//COLLISIONSylinder
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("OverlappedComp : %s"), *OtherComp->GetName()));

		if (FString("RIGHT_SHIELD_GUARD_COLLISION") == OtherCompName || FString("LEFT_SHIELD_GUARD_COLLISION") == OtherCompName)
		{
			++GuardCount;
			SwordAnimInstance->SetGuard(true);

			float ArmReflectPower = -1000.f;
			FVector dir = Player->GetActorLocation() - GetActorLocation();
			dir.Normalize();
			if (FString("LEFT_SHIELD_GUARD_COLLISION") == OtherCompName)
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);
				for (auto FA : FoundActors)
				{
					Player = Cast<AShieldManCharacter>(FA);
				}
				Player->GetMesh()->AddImpulseToAllBodiesBelow(dir * ArmReflectPower * 100, TEXT("Bip001-L-Forearm"));

				//auto t = dir * ArmReflectPower * 1000;
				//UE_LOG(LogTemp, Log, TEXT("LEFT t : %s"), *t.ToString());
			}
			else
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);
				for (auto FA : FoundActors)
				{
					Player = Cast<AShieldManCharacter>(FA);
				}
				Player->GetMesh()->AddImpulseToAllBodiesBelow(dir * ArmReflectPower * 100, TEXT("Bip001-R-Forearm"));
				auto t = dir * ArmReflectPower * 1000;
				//UE_LOG(LogTemp, Log, TEXT("Right t : %s"), *t.ToString());
			}

			ServerStartNiagaraEffect();
		}
		else if (FString("CollisionCylinder") == OtherCompName)
		{
			Player->DecreaseHP(20.f);

			FVector dir = Player->GetActorLocation() - GetActorLocation();
			dir.Normalize();
			float BodyReflectPower = -1000.f;
			if (rand() % 2 == 1)
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);
				for (auto FA : FoundActors)
				{
					Player = Cast<AShieldManCharacter>(FA);
				}
				Player->GetMesh()->AddImpulseToAllBodiesBelow(dir * BodyReflectPower * 100, TEXT("Bip001-L-UpperArm"));
			}
			else
			{
				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShieldManCharacter::StaticClass(), FoundActors);
				for (auto FA : FoundActors)
				{
					Player = Cast<AShieldManCharacter>(FA);
				}
				Player->GetMesh()->AddImpulseToAllBodiesBelow(dir * BodyReflectPower * 100, TEXT("Bip001-R-UpperArm"));
			}
			ServerStartNiagaraEffect();

			if (true == Player->isDeath())
			{
				PrimaryActorTick.SetTickFunctionEnable(false);
				Destroy();
			}
		}
		isHit = true;
	}
	else
	{

	}
}
void ASM_Sword::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Role == ROLE_Authority)
	{
		ServerOverlapBegin(OtherComp);
	}

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
		//UE_LOG(LogTemp, Log, TEXT("SwordAnimInstance not mapping"));
	}
}


void ASM_Sword::Begin_StageOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (EventTrigger == false)
	{
		//UE_LOG(LogTemp, Log, TEXT("Begin_StageOverlapBegin"));

		OtherActor->SetActorLocation(Startto);
		EventTrigger = true;
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
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

void ASM_Sword::ServerStartNiagaraEffect_Implementation()
{
	StartNiagaraEffect();
}