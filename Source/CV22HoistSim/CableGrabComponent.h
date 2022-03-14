// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CableGrabComponent.generated.h"

/**
 *
 */
UCLASS()
class CV22HOISTSIM_API UCableGrabComponent : public USceneComponent {
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;


public:
	UCableGrabComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Setup(class UCableComponent* cable);

	void Grab(class UPrimitiveComponent* hand);
	void Release();

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
