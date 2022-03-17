// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HandControllerComponent.h"
#include "TailScanner.generated.h"

UCLASS()
class CV22HOISTSIM_API ATailScanner : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATailScanner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveToNextBeAt();

	UFUNCTION(BlueprintCallable)
	void ResetView();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* CurrentBeAtActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* CurrentBeAtComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture* SpectatorTexture = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool NVGsOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHandControllerComponent* LeftController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHandControllerComponent* RightController;

	UPROPERTY()
	class UPostProcessComponent* PostProcessComponent;

	UPROPERTY()
	class UMaterialInstanceDynamic* NVGMaterialInstance;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface * NVGMaterialBase;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ToggleNVGs();

	UFUNCTION(BlueprintCallable)
	void GrabLeft();

	UFUNCTION(BlueprintCallable)
	void ReleaseLeft();

	UFUNCTION(BlueprintCallable)
	void GrabRight();

	UFUNCTION(BlueprintCallable)
	void ReleaseRight();

	class UCanBeAtMovementComponent* GetCanBeAtMovementComponent();

	void MoveForward(float amount);
	void MoveRight(float amount);
	void MoveUp(float amount);
	void YawRight(float amount);

	void HoistSetPower(bool on);
	void HoistSetJettison(bool state);
	void HoistSetUpDown(float amount);
	
	void HoistUpDown(float amount);
	void HoistPower();
	void HoistJettison();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentBeAtActor() { return CurrentBeAtActor; }

	UFUNCTION(BlueprintCallable)
	UHandControllerComponent* GetRightController() { return RightController; }

	UFUNCTION(BlueprintCallable)
	UHandControllerComponent* GetLeftController() { return LeftController; }

private:
	FRotator GetAttachedSocketRotation();
	void AffectMoveToNext(AActor* actor);
};
