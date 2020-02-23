// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ControlMode.h"

/**
 * 
 */
class SHIELDMAN_API RHandControl : public ControlMode
{
public:
	RHandControl();
	~RHandControl();

public:
	virtual void Initialize();
};
