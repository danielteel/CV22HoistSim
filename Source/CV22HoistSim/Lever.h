// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeverHandle.h"
#include "Components/SceneComponent.h"
#include "Lever.generated.h"


UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeverWasChanged, int, value);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CV22HOISTSIM_API ULever : public USceneComponent
{
	GENERATED_BODY()

public:
	ULever();
	virtual void OnRegister() override;

	UPROPERTY(BlueprintAssignable)
	FLeverWasChanged OnLeverWasChanged;

	void LeverWasSet(float value);

	UFUNCTION(BlueprintCallable)
	void SetLeverValue(int value);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LeverHandleMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BaseMesh = nullptr;

	UPROPERTY()
	ULeverHandle* LeverHandleComponent = nullptr;
	UPROPERTY()
	UStaticMeshComponent* BaseComponent = nullptr;


	UPROPERTY(EditAnywhere, Category = "Setup", BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	int Positions = 2;

	UPROPERTY(EditAnywhere, Category = "Setup", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool WantsConstantUpdates = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int CurrentValue = 0;

	float MinPitch = -80;
	float MaxPitch = 80;
};
