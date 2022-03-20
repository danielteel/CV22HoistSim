// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RescueHook.h"
#include "Components/SphereComponent.h"
#include "HoistComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CV22HOISTSIM_API UHoistComponent : public USceneComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHoistComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool IsWithinLimit();


	UFUNCTION(BlueprintCallable)
		UPrimitiveComponent* GetAttachedDevice();

	UFUNCTION(BlueprintCallable)
		void Extend(float value);

	UFUNCTION(BlueprintCallable)
		float GetHoistOutLength();

	UFUNCTION(BlueprintCallable)
		void JettisonHoist();

	UFUNCTION(BlueprintCallable)
		bool IsJettisoned();

	UFUNCTION(BlueprintCallable)
		TArray<class UPrimitiveComponent*> GetWindAffectableComponents();

	UFUNCTION(BlueprintCallable)
		bool IsAHoistGrabbableComponent(USceneComponent* component);

	UFUNCTION(BlueprintCallable)
		class UPrimitiveComponent* GetDeviceOnGround();

	USceneComponent* GetCableBase() { return CableBase; }
	void GrabEndRescueHook();

private:
	void FixStuckCable();
	void UpdateCableGrab(float deltaTime);
	void SetHoistLength(float hoistLength);

	float GrabSpeed = 400.0f;//1 ft/second
	bool GrabConditionMet = false;
	float GrabConditionDelta = 1.0f;//when this close, just snap to hand or boom head
	bool LastGrabState = false;
	float HoistGrabOffset = 0.0f;

	float CableWindForce = 400.0f;
	double lastCableUpdateTime = 0;

	float HoistLimitDist = 426.72f;
	float HoistMaxLength = 7620.0f;
	float HoistMaxSpeed = 152.4f;
	float HoistLimitMaxSpeed = 38.1f;

	float HoistOutLength = 0.0f;
	float ExtendCommand = 0.0f;
	bool Jettisoned = false;

	FVector LastPosition;

protected:
	UPROPERTY()
	class UStaticMeshComponent* AttachedDevice = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* RescueHookMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* BoomHeadMesh = nullptr;

	UStaticMeshComponent* BoomHead = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CableBase = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCableComponent* BaseToHookCable = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCableComponent* BoomToBaseCable = nullptr;

	UPROPERTY(VisibleAnywhere)
		class UCableGrabComponent* CableGrabber = nullptr;

	URescueHook* RescueHook = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* CableBaseConstraint = nullptr;
};
