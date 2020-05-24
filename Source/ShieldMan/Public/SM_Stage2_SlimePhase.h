// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_Stage2_SlimePhase.generated.h"

UCLASS()
class SHIELDMAN_API ASM_Stage2_SlimePhase : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OpenCollision;
	UPROPERTY(Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* StartCollision;

	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* CeilingRock;

	TArray<class AMetaBall_Slime*> m_SlimeArr;

	class ASM_Stage2_Door* Entrance;
	class ASM_Stage2_Door* Exit;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf< class ASM_Stage2_Door> DoorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf< class AMetaBall_Slime> SlimeClass;

public:	
	// Sets default values for this actor's properties
	ASM_Stage2_SlimePhase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOpenOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnStartOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
