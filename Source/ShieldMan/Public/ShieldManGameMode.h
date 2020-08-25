// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ShieldMan.h"
#include "GameFramework/GameModeBase.h"
#include "ShieldManGameMode.generated.h"

UCLASS(minimalapi)
class AShieldManGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	int32 PlayerNum;

public:
	AShieldManGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Class)
	TSubclassOf<class AShieldManCharacter> MainCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Class)
	TSubclassOf<class ASM_PlayerPawn> ArmCharacterClass;

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void PostLogin(APlayerController* NewPlayer);
	void Logout(AController* Exiting);
};



