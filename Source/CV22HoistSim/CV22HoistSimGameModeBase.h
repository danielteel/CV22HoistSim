// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CV22HoistSimGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CV22HOISTSIM_API ACV22HoistSimGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsNight = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float DustFactor=0.5f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float MosyFactor = 0.5f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float KickFactor = 0.5f;
};
