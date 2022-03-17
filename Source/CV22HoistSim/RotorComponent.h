// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RotorComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CV22HOISTSIM_API URotorComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	URotorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	class UHoistComponent * GetHoistComponent();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	bool IsLeftRotor = false;

private:

	//Direction the wind is coming from in degrees, world space
	float WindDirection = 0.0f;
	//Wind Speed in m/s, 5.144 = 10knots
	float WindSpeed = 20;
	float DiscRadius = 300.0f;
	float DownwashZeroHeight = 15000.0f;//90 meters=295feet
	float DownwashRadius = 1500.0f;
	float DownwashSpeedAtRotors = 1800.0f;
	float DownwashMaxGust = 1500.0f;
	float DownwashCurrentGust = 0.0f;
	float DownwashSpeedReduceFactor = (DownwashSpeedAtRotors + DownwashMaxGust) / DownwashZeroHeight;

	FVector GetWindForce(UPrimitiveComponent* component, FVector windDirection, float windSpeed);
	void ApplyWindForces();
	float GetWindSurfaceArea(UPrimitiveComponent* primitive);
};
