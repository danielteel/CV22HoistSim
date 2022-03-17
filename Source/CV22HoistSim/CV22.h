// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoistComponent.h"
#include "RotorComponent.h"
#include "HoistControlPanel.h"
#include "CanBeAt.h"
#include "CV22.generated.h"

UCLASS()
class CV22HOISTSIM_API ACV22 : public AActor, public ICanBeAt
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACV22();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateRotorDust(class UParticleSystemComponent * dust, FVector rotorPosition);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CanBeAt")
	USceneComponent* GetComponentToAttachTo();
	virtual USceneComponent* GetComponentToAttachTo_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CanBeAt")
	void EnteredActor(class APawn* pawn);
	virtual void EnteredActor_Implementation(class APawn* pawn) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CanBeAt")
	void LeftActor(class APawn* pawn);
	virtual void LeftActor_Implementation(class APawn* pawn) override;

protected:

	UPROPERTY(VisibleAnywhere)
	class ATailScanner* TailScannerInsideMe = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	class UStaticMeshComponent* Body = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	URotorComponent* RotorLeft = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	URotorComponent* RotorRight = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	class UParticleSystemComponent* DustLeft = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	class UParticleSystemComponent* DustRight = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	class UAudioComponent* AircraftNoise = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UCV22MovementComponent* MovementComponent = nullptr;

};
