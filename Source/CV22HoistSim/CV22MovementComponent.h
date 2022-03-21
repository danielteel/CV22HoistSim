// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CanBeAtMovementComponent.h"
#include "CV22MovementComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CV22HOISTSIM_API UCV22MovementComponent : public UCanBeAtMovementComponent
{
	GENERATED_BODY()

public:
	UCV22MovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void MoveForward(float val) override;
	virtual void MoveRight(float val) override;
	virtual void MoveUp(float val) override;
	virtual void YawRight(float val) override;

	UFUNCTION(BlueprintCallable, Category=Movement)
		FVector GetVelocity() { return LastVelocity; }

	UFUNCTION(BlueprintCallable, Category = Movement)
	int32 GetAGL();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Setup(UPrimitiveComponent* aircraft);

protected:
	UPrimitiveComponent* Aircraft = nullptr;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	FVector4 MoveIntent = FVector4(0.0f, 0.0f, 0.0f, 0.0f);//X, Y, Z are translation intents, W is yaw intent

	float Roll = 0;
	float Pitch = 0;
	float CurrentRoll = 0;
	float CurrentPitch = 0;

	UPROPERTY(VisibleAnywhere)
	FVector4 LastVelocity = FVector4(0.0f, 0.0f, 0.0f, 0.0f);

	int32 AGL = 0;
};
