// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Lever.generated.h"


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeverWasChanged, int, value);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CV22HOISTSIM_API ULever : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULever();


	UPROPERTY(BlueprintAssignable)
	FLeverWasChanged OnLeverWasChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	class ULeverHandle* LeverHandleComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	class UStaticMeshComponent* BaseMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	int Positions = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentValue = 0;

	float MinPitch = -80;
	float MaxPitch = 80;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LeverWasSet(float value);

	UFUNCTION(BlueprintCallable)
	void SetLeverValue(int value);
};
