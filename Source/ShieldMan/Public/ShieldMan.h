// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include "EngineMinimal.h"

using namespace std;

enum EControlMode {
	BodyControlMode,
	RHandControlMode,
	LHandControlMode
};

enum CharacterStatus {
	PossibleMove,
	ImpossibleMove,
	Death,
};

/*
�α� ���

UE_LOG(LogTemp, Warning, TEXT("This is test"));

UE_LOG(LogTemp, Warning, TEXT("%f"), Player->GetActorLocation().X);

UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());

/////////////////////////////////////////////////////////////

Debug Logging Library �÷����� 

ULog::Hello();

ULog::Hello(LO_Viewport);

ULog::Invalid("Can't equip Shield", "", LO_Viewport);

ULog::Number(5, "The Number is: ");

ULog::Vector(GetActorLocation(), "MyActor location: ", "", LO_Viewport);

//���� �Ҹ� ���� Ŭ����
ULog::Info(CUR_CLASS);

//���� �Ҹ� Ŭ������ �Լ�
ULog::Info(CUR_CLASS_FUNC);

//���� �Ҹ� Ŭ����, �Լ�, ��� ° ��������
ULog::Info(CUR_CLASS_FUNC_WITH_LINE);

//���� �Ҹ� ���� �Լ� �ñ׳���?
ULog::Info(CUR_FUNC_SIG);

....etc ���� �������


 ��ť��Ʈ
http://alielsaleh.com/tools/UE4-Plugins/DebugLogLibrary/
		Build.cs�� �߰��ؾ� ��
	PrivateDependencyModuleNames.AddRange(new string[] { "Debug"});
*/