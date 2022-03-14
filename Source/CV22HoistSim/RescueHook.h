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

protected:
	virtual void BeginPlay() override;

private:
	class UPhysicsHandleComponent* GrabComponent = nullptr;

	class UPhysicsConstraintComponent* HookConstraint = nullptr;

	class UPrimitiveComponent* AttachedDevice = nullptr;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
		bool GrabStart(class UPrimitiveComponent* hand);//Return false if object doesnt want grabbed
	virtual bool GrabStart_Implementation(class UPrimitiveComponent* hand) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
		bool GrabEnd(class UPrimitiveComponent* hand);
	virtual bool GrabEnd_Implementation(class UPrimitiveComponent* hand) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
		bool GrabEvent(class UPrimitiveComponent* hand, bool buttonPressed, float upDown, float leftRight);
	virtual bool GrabEvent_Implementation(class UPrimitiveComponent* hand, bool buttonPressed, float xAxis, float yAxis) override;

	UFUNCTION(BlueprintCallable, Category = "Rescue Hook")
		void Attach(UPrimitiveComponent * device);

	UFUNCTION(BlueprintCallable, Category = "Rescue Hook")
		UPrimitiveComponent * GetAttachedDevice();

	UFUNCTION(BlueprintCallable, Category = "Rescue Hook")
		void Dettach();
};
