// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "HandControllerComponent.generated.h"

/**
 * 
 */
UCLASS()
class CV22HOISTSIM_API UHandControllerComponent : public UMotionControllerComponent
{
	GENERATED_BODY()

public:
	UHandControllerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetOtherHand(UHandControllerComponent * Hand);

	UFUNCTION(BlueprintCallable, Category = "Hand")
	void Grab();

	UFUNCTION(BlueprintCallable, Category = "Hand")
	void ReleaseGrab();

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetEnabled(bool enabled);

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool GetEnabled() { return Enabled; }

protected:
	UPROPERTY(EditAnywhere)
	float GrabDistance = 30.0f;

	UPROPERTY(VisibleAnywhere)
	bool Enabled = true;

	UPROPERTY()
	UHandControllerComponent* OtherHand = nullptr;

	UPROPERTY()
	class UPrimitiveComponent* GrabbedComponent = nullptr;
};
