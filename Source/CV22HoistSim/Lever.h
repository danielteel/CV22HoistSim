// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Lever.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CV22HOISTSIM_API ULever : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULever();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	class ULeverHandle* LeverHandleComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	class UStaticMeshComponent* BaseMeshComponent = nullptr;

	UPROPERTY(EditAnywhere)
	int Positions = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentPosition = 0;
	float CurrentLeverValue = 0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LeverSetToValue(float value);

	UFUNCTION(BlueprintCallable)
	float GetLeverValue() { return CurrentLeverValue; }
};
