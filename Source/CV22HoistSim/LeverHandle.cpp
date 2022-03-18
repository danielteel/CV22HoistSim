// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverHandle.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Quat.h"
#include "Lever.h"

// Called when the game starts
void ULeverHandle::BeginPlay() {
	Super::BeginPlay();

	InitialRotation = GetRelativeRotation();
}

void ULeverHandle::Setup(float minPitch, float maxPitch, float initialValue, ULever* owner) {
	MinPitch = minPitch;
	MaxPitch = maxPitch;
	Owner = owner;
	SetValue(initialValue);
}

void ULeverHandle::SetPitch(float pitch) {
	pitch = FMath::Clamp(pitch, MinPitch, MaxPitch);
	SetRelativeRotation(FRotator(pitch, 0.0f, 0.0f));
	CurrentPitch = pitch;
}

void ULeverHandle::SetValue(float value) {
	SetPitch(FMath::Lerp(MinPitch, MaxPitch, value));
}

USceneComponent* ULeverHandle::GetComponentToGrab_Implementation() {
	return this;
}

void ULeverHandle::GrabStart_Implementation(UPrimitiveComponent * hand) {
}

void ULeverHandle::GrabEnd_Implementation(UPrimitiveComponent * hand) {
	if (Owner) {
		float val = 1.0f - (MaxPitch - CurrentPitch) / (MaxPitch - MinPitch);
		Owner->LeverWasSet(val);
	}
}

void ULeverHandle::GrabEvent_Implementation(UPrimitiveComponent * hand, bool buttonPressed, float xAxis, float yAxis) {
	FVector inverseTransform = UKismetMathLibrary::InverseTransformLocation(GetAttachParent()->GetComponentTransform(), hand->GetComponentLocation());
	FVector unrotatedVector = FRotator(90.f, 0.0f, 0.0f).UnrotateVector(inverseTransform);
	float value = UKismetMathLibrary::Atan2(unrotatedVector.Z, unrotatedVector.X);

	SetPitch(FMath::RadiansToDegrees(value));
}