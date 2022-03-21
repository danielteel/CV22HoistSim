// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Grabbable.h"
#include "LeverHandle.generated.h"

/**
 * 
 */
UCLASS()
class CV22HOISTSIM_API ULeverHandle : public UStaticMeshComponent, public IGrabbable
{
	GENERATED_BODY()

public:
	ULeverHandle();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetValue(float value);
	void Setup(float minPitch, float maxPitch, float initialValue, class ULever* owner, bool wantsConstantUpdates);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable") USceneComponent* GetComponentToGrab();
	virtual USceneComponent* GetComponentToGrab_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable") void GrabStart(class UPrimitiveComponent* hand);
	virtual void GrabStart_Implementation(class UPrimitiveComponent* hand) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable") void GrabEnd(class UPrimitiveComponent* hand);
	virtual void GrabEnd_Implementation(class UPrimitiveComponent* hand) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable") void GrabEvent(class UPrimitiveComponent* hand, bool buttonPressed, float upDown, float leftRight);
	virtual void GrabEvent_Implementation(class UPrimitiveComponent* hand, bool buttonPressed, float xAxis, float yAxis) override;

private:
	UPROPERTY()
	class ULever* Owner = nullptr;

	FVector GrabOffset;

	float MinPitch = 0;
	float MaxPitch = 0;

	bool WantsConstantUpdates = false;

	float CurrentPitch = 0;
	float TargetPitch = 0;
	float PitchAnimSpeed = 500.0f;
};
