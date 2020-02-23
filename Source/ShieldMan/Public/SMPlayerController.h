// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
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
protected:
	virtual void BeginPlay() override;

};
