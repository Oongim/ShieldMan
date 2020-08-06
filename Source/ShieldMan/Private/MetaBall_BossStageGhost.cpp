// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaBall_BossStageGhost.h"
#include "WidgetComponent.h"

void AMetaBall_BossStageGhost::BeginPlay()
{
	Super::BeginPlay();
	HPBarWidget->SetVisibility(false);
	bAttack = false;
}

void AMetaBall_BossStageGhost::Tick(float DeltaTime)
{
	if (!GetAlive()) {
		if (GetOpacity() <= -0.1f) {
			Destroy();
		}
	}
	if (GetStatus() == STATUS::ATTACKING) {
		bAttack = true;
	}
	Super::Tick(DeltaTime);
	if (bAttack) {
		if (GetStatus() == STATUS::MOVING) {
			SetAlive(false);
		}
	}
	
}
void AMetaBall_BossStageGhost::OnRepeatTimer()
{
	if (!GetAlive()) return;

	FVector NextLocation;

		NextLocation = {
				FMath::FRandRange(-840.f, 840.f),
				FMath::FRandRange(-800.f, 200.f),
				1000.f
		};
	SetDestination(NextLocation);
	AddForceToVelocity(10);

	SetStatus(STATUS::MOVING);

}

void AMetaBall_BossStageGhost::MoveStart()
{
	Super::MoveStart();
	float RepeatInterval = 3.0f;
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMetaBall_BossStageGhost::OnRepeatTimer, RepeatInterval, false);
}