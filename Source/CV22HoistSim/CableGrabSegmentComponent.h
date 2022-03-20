// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Grabbable.h"
#include "CableGrabSegmentComponent.generated.h"

/**
 *
 */
UCLASS()
class CV22HOISTSIM_API UCableGrabSegmentComponent : public UCapsuleComponent, public IGrabbable {
	GENERATED_BODY()

public:
	UCableGrabSegmentComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
		USceneComponent* GetComponentToGrab();
	virtual USceneComponent* GetComponentToGrab_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
		void GrabStart(class UPrimitiveComponent* hand);
	virtual void GrabStart_Implementation(class UPrimitiveComponent* hand) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
		void GrabEnd(class UPrimitiveComponent* hand);
	virtual void GrabEnd_Implementation(class UPrimitiveComponent* hand) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
		void GrabEvent(class UPrimitiveComponent* hand, bool buttonPressed, float upDown, float leftRight);
	virtual void GrabEvent_Implementation(class UPrimitiveComponent* hand, bool buttonPressed, float xAxis, float yAxis) override;

private:
	UPROPERTY(VisibleAnywhere)
	float InitialCapsuleRadius = .25f;

	UPROPERTY(VisibleAnywhere)
	float InitialCapsuleHalfHeight = 1.0f;
};
