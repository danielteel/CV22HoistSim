// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Grabbable.h"
#include "RescueHook.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CV22HOISTSIM_API URescueHook : public UStaticMeshComponent, public IGrabbable {
	GENERATED_BODY()

public:

	URescueHook();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister() override;

protected:
	virtual void BeginPlay() override;

private:

	class UPrimitiveComponent* AttachedDevice = nullptr;

	UPROPERTY()
	class UHoistComponent* HoistOwner = nullptr;

	FVector LastLocation;
	FVector Velocity;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Grabbable")
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


	UFUNCTION(BlueprintCallable, Category = "Rescue Hook")
		UPrimitiveComponent * GetAttachedDevice();

};
