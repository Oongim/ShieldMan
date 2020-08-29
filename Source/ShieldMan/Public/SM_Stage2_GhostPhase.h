       // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/Actor.h"
#include "SM_Stage2_GhostPhase.generated.h"

UCLASS()
class SHIELDMAN_API ASM_Stage2_GhostPhase : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* OpenCollision;
	UPROPERTY(Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* StartCollision;

	TArray<class AMetaBall_Ghost*> m_GhostArr;

	class ASM_Stage2_Door* Entrance;
	class ASM_Stage2_Door* Exit;

	bool bOpened;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf< class ASM_Stage2_Door> DoorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf< class AMetaBall_Ghost> GhostClass;

public:
	// Sets default values for this actor's properties
	ASM_Stage2_GhostPhase();

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

	UFUNCTION(Server, Reliable, WithValidation)
		virtual void ServerOpenDoor();

	UFUNCTION(NetMulticast, Reliable)
		virtual void ServerCloseDoor();

	UFUNCTION(NetMulticast, Reliable)
		void ServerSpawnActor();


};
