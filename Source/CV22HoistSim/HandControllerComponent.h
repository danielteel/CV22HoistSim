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
	virtual void OnRegister() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateHighlight();

	UPrimitiveComponent * GetComponentToGrab();

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

	UFUNCTION(BlueprintCallable, Category = "Hand")
	USceneComponent* GetGrabbedComponent() { return GrabbedComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, Category="Highlight")
	UMaterial* HighlightMaterial = nullptr;

	UPROPERTY()
	UStaticMeshComponent* HighlightStaticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	float GrabDistance = 30.0f;

	UPROPERTY(VisibleAnywhere)
	bool Enabled = true;

	UPROPERTY()
	UHandControllerComponent* OtherHand = nullptr;

	UPROPERTY()
	class USceneComponent* GrabbedComponent = nullptr;

	UPROPERTY()
		class USceneComponent* HighlightedComponent = nullptr;
};
