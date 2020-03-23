// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
//#include "GameFramework/Controller.h"
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
	/* UMG 에셋 레퍼런스*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/*위젯을 만든 후 유지하는 변수*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;


protected:
	virtual void BeginPlay() override;


private:
	//UPROPERTY()
	//AController* Controller1;
};
