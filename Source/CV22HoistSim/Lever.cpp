// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

ULever::ULever() {
	LeverHandleComponent = CreateDefaultSubobject<ULeverHandle>(FName("Handle"));
	BaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Base"));
}


void ULever::OnRegister() {
	Super::OnRegister();

	if (BaseComponent) {
		if (BaseMesh) {
			BaseComponent->SetStaticMesh(BaseMesh);
		} else {
			UE_LOG(LogTemp, Warning, TEXT("No base mesh selected for Lever component"))
		}
		BaseComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (LeverHandleComponent) {
		if (LeverHandleMesh) {
			LeverHandleComponent->SetStaticMesh(LeverHandleMesh);
		} else {
			UE_LOG(LogTemp, Warning, TEXT("No lever handle mesh selected for Lever component"))
		}
		LeverHandleComponent->AttachToComponent(BaseComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Lever"));
		LeverHandleComponent->Setup(MinPitch, MaxPitch, 0.0f, this);
	}
}


void ULever::SetLeverValue(int value) {
	value = FMath::Clamp(value, 0, Positions-1);
	float leverAlpha = float(value) / (float(Positions) - 1);
	CurrentValue = value;
	LeverHandleComponent->SetValue(leverAlpha);
	OnLeverWasChanged.Broadcast(CurrentValue);
}

void ULever::LeverWasSet(float value) {
	float delta = MaxPitch - MinPitch;
	float divider = delta / float(Positions);
	int step = FMath::Clamp(int((value * delta) / divider), 0, Positions-1);
	float leverAlpha = float(step) / (float(Positions) - 1);
	CurrentValue = step;
	LeverHandleComponent->SetValue(leverAlpha);

	OnLeverWasChanged.Broadcast(CurrentValue);
}