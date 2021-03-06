// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanBeAt.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCanBeAt : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CV22HOISTSIM_API ICanBeAt
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CanBeAt")
	USceneComponent* GetComponentToAttachTo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CanBeAt")
	void EnteredActor(class APawn* pawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CanBeAt")
	void LeftActor(class APawn* pawn);

};
