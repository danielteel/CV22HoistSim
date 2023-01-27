// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HoistSim_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CV22HOISTSIM_API UHoistSim_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	FProcHandle nodeProcHandle;
};
