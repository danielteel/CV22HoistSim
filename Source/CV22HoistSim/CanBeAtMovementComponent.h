// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "CanBeAtMovementComponent.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class CV22HOISTSIM_API UCanBeAtMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	virtual void MoveForward(float val) { unimplemented(); };
	virtual void MoveRight(float val) { unimplemented(); };
	virtual void MoveUp(float val) { unimplemented(); };
	virtual void YawRight(float val) { unimplemented(); };
};
