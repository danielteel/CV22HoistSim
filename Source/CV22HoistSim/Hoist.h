// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hoist.generated.h"

UCLASS()
class CV22HOISTSIM_API AHoist : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoist();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Base = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Hook = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* BaseMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* HookMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPhysicsConstraintComponent* HookConstraint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCableComponent* Cable = nullptr;

	UPROPERTY(VisibleAnywhere)
	class UCableGrabComponent* CableGrabber = nullptr;
};
