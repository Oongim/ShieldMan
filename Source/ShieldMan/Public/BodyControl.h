// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ControlMode.h"

/**
 * 몸통 조작 클래스
 * 컨트롤 모드를 상속받음
 */
class SHIELDMAN_API BodyControl : public ControlMode
{
public:
	BodyControl();
	~BodyControl();

public:
	virtual void Initialize();
};
