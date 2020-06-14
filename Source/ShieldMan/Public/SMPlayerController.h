// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "SM_PlayerPawn.h"
#include "GameFramework/PlayerController.h"
#include "SMPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHIELDMAN_API ASMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/* UMG ���� ���۷���*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/*������ ���� �� �����ϴ� ����*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

	ASM_PlayerPawn* player1;
	ASM_PlayerPawn* player2;
	ASM_PlayerPawn* player3;

protected:
	virtual void BeginPlay() override;

};
