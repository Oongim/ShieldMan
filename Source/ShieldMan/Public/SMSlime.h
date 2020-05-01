// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "SMSlime.generated.h"

UCLASS()
class SHIELDMAN_API ASMSlime : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASMSlime();

public:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = AI)
		const float GetHealth();


	UPROPERTY(EditAnywhere, Category = Pawn)
		class UBehaviorTree* BehaviorTree;

private:
	UPROPERTY()
		float Health;
};
