// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoistComponent.h"
#include "RotorComponent.h"
#include "HoistControlPanel.h"
#include "CV22.generated.h"

UCLASS()
class CV22HOISTSIM_API ACV22 : public AActor
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


protected:

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
