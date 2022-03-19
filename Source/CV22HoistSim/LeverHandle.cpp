// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverHandle.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Quat.h"
#include "Lever.h"

// Called when the game starts

ULeverHandle::ULeverHandle() {
	PrimaryComponentTick.bCanEverTick = true;
}
void ULeverHandle::BeginPlay() {
	Super::BeginPlay();

	InitialRotation = GetRelativeRotation();
}

void ULeverHandle::Setup(float minPitch, float maxPitch, float initialValue, ULever* owner) {
	MinPitch = minPitch;
	MaxPitch = maxPitch;
	Owner = owner;

	CurrentPitch = FMath::Lerp(MinPitch, MaxPitch, initialValue);
	TargetPitch = FMath::Lerp(MinPitch, MaxPitch, initialValue);
	SetRelativeRotation(FRotator(CurrentPitch, 0.0f, 0.0f));
}


void ULeverHandle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float pitchDelta = CurrentPitch - TargetPitch;
	if (!FMath::IsNearlyZero(pitchDelta)) {
		float moveAmount = pitchDelta > 0 ? -PitchAnimSpeed * DeltaTime : PitchAnimSpeed * DeltaTime;

		if (FMath::Abs(moveAmount) > FMath::Abs(pitchDelta)) {
			moveAmount = FMath::Abs(pitchDelta)*FMath::Sign(moveAmount);
		}

		CurrentPitch += moveAmount;

		SetRelativeRotation(FRotator(CurrentPitch, 0.0f, 0.0f));
	}
}

void ULeverHandle::SetValue(float value) {
	TargetPitch = FMath::Lerp(MinPitch, MaxPitch, value);
}

USceneComponent* ULeverHandle::GetComponentToGrab_Implementation() {
	return this;
}

void ULeverHandle::GrabStart_Implementation(UPrimitiveComponent * hand) {
	GrabOffset = GetSocketLocation(FName("Handle")) - hand->GetComponentLocation();
}

void ULeverHandle::GrabEnd_Implementation(UPrimitiveComponent * hand) {
	if (Owner) {
		float val = 1.0f - (MaxPitch - CurrentPitch) / (MaxPitch - MinPitch);
		Owner->LeverWasSet(val);
	}
}

void ULeverHandle::GrabEvent_Implementation(UPrimitiveComponent * hand, bool buttonPressed, float xAxis, float yAxis) {
	FVector inverseTransform = UKismetMathLibrary::InverseTransformLocation(GetAttachParent()->GetComponentTransform(), hand->GetComponentLocation()+GrabOffset);
	FVector unrotatedVector = FRotator(90.f, 0.0f, 0.0f).UnrotateVector(inverseTransform);
	float pitch = FMath::RadiansToDegrees(UKismetMathLibrary::Atan2(unrotatedVector.Z, unrotatedVector.X));
	pitch = FMath::Clamp(pitch, MinPitch, MaxPitch);
	CurrentPitch = pitch;
	TargetPitch = pitch;
	SetRelativeRotation(FRotator(pitch, 0.0f, 0.0f));
}