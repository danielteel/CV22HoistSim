// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

ULever::ULever() {
}


void ULever::OnRegister() {
	Super::OnRegister();

	BaseComponent = NewObject<UStaticMeshComponent>(this, FName("Base"));
	BaseComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BaseComponent->RegisterComponent();
	BaseComponent->SetStaticMesh(BaseMesh);

	LeverHandleComponent = NewObject<ULeverHandle>(this, FName("Handle"));
	LeverHandleComponent->AttachToComponent(BaseComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	LeverHandleComponent->RegisterComponent();
	LeverHandleComponent->SetStaticMesh(LeverHandleMesh);
	LeverHandleComponent->Setup(MinPitch, MaxPitch, 0.0f, this, WantsConstantUpdates);
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
	if (CurrentValue != step) {
		CurrentValue = step;
		OnLeverWasChanged.Broadcast(CurrentValue);
	}
	LeverHandleComponent->SetValue(leverAlpha);
}