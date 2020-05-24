// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShieldMan.h"
#include "Blueprint/UserWidget.h"
#include "UI_TitleLobbyScene.generated.h"

/**
 *
 */
UCLASS()
class SHIELDMAN_API UUI_TitleLobbyScene : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize();
private:
	FText Player1_Name;
	FText Player2_Name;
	FText Player3_Name;

	bool bReadyRight;
	bool bReadyBody;
	bool bReadyLeft;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ChoiceButton_Right;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ChoiceButton_Body;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ChoiceButton_Left;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Ready_Right;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Ready_Body;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Ready_Left;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Player1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Player2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Player3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ChoiceText_Right;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ChoiceText_Body;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ChoiceText_Left;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ReadyText_Right;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ReadyText_Body;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ReadyText_Left;

public:
	UFUNCTION()
		void ChoiceButton_Right_Clicked();

	UFUNCTION()
		void ChoiceButton_Body_Clicked();

	UFUNCTION()
		void ChoiceButton_Left_Clicked();

	UFUNCTION()
		void Ready_Right_Clicked();

	UFUNCTION()
		void Ready_Body_Clicked();

	UFUNCTION()
		void Ready_Left_Clicked();
};
