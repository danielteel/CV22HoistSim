// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "CV22MovementComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CV22HOISTSIM_API UCV22MovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	UCV22MovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveForward(float val);
	void MoveRight(float val);
	void MoveUp(float val);
	void YawRight(float val);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	FVector4 MoveIntent = FVector4(0.0f, 0.0f, 0.0f, 0.0f);//X, Y, Z are translation intents, W is yaw intent

	UPROPERTY(VisibleAnywhere)
	FVector4 CurrentSpeed = FVector4(0.0f, 0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere)
	FVector4 MaxPositiveSpeed = FVector4(3000.0f, 1500.0f, 1000.0f, 30.0f);//3000 forwards, 1500 right, 508 up, 10 degrees yaw/sec right
	UPROPERTY(VisibleAnywhere)
	FVector4 MaxNegativeSpeed = FVector4(-1000.0f, -1500.0f, -1000.0f, -30.0f);//1000 backwards, 1500 left , 508 down, 10 degrees yaw/sec left

	UPROPERTY(VisibleAnywhere)
	FVector4 MaxAcceleration = FVector4(1500.0f, 1500.0f, 750.f, 20.0f);

	UPROPERTY(VisibleAnywhere)
	FVector4 DecayRate = FVector4(100.0f, 100.0f, 100.0f, 5.0f);


};
