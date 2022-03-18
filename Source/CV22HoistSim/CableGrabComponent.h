// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grabbable.h"
#include "CableGrabComponent.generated.h"

/**
 *
 */
UCLASS()
class CV22HOISTSIM_API UCableGrabComponent : public USceneComponent, public IGrabbable {
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;


public:
	UCableGrabComponent();
	
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

	void Setup(class UCableComponent* cable);

	bool IsGrabbed();

	class UPrimitiveComponent* GrabbedHand = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	class UCableComponent* MasterCable = nullptr;

	UPROPERTY(VisibleAnywhere)
	int32 NumCableSegments = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<class UCableGrabSegmentComponent*> CableSegments;

	UPROPERTY(VisibleAnywhere)
	bool Grabbed = false;

	class UCableGrabSegmentComponent* NewGrabSegment();
};
