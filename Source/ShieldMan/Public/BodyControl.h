// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ControlMode.h"

/**
 * ���� ���� Ŭ����
 * ��Ʈ�� ��带 ��ӹ���
 */
class SHIELDMAN_API BodyControl : public ControlMode
{
public:
	BodyControl();
	~BodyControl();

public:
	virtual void Initialize();
};
