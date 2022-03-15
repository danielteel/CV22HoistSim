// Fill out your copyright notice in the Description page of Project Settings.


#include "CV22MovementComponent.h"
#include "CV22.h"

UCV22MovementComponent::UCV22MovementComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UCV22MovementComponent::BeginPlay() {
	Super::BeginPlay();
}

void UCV22MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACV22* owner = Cast<ACV22>(GetOwner());
	if (!owner) return;

	FVector4 speedDelta;
	speedDelta.X = MoveIntent.X*MaxAcceleration.X*DeltaTime;
	speedDelta.Y = MoveIntent.Y*MaxAcceleration.Y*DeltaTime;
	speedDelta.Z = MoveIntent.Z*MaxAcceleration.Z*DeltaTime;
	speedDelta.W = MoveIntent.W*MaxAcceleration.W*DeltaTime;


	if (FMath::IsNearlyZero(speedDelta.X)) {
		if (!FMath::IsNearlyEqual(FMath::Sign(CurrentSpeed.X - DecayRate.X*DeltaTime), FMath::Sign(CurrentSpeed.X))) {
			CurrentSpeed.X = 0;
		} else {
			CurrentSpeed.X -= FMath::Sign(CurrentSpeed.X) * DecayRate.X*DeltaTime;
		}
	}
	if (FMath::IsNearlyZero(speedDelta.Y)) {
		if (!FMath::IsNearlyEqual(FMath::Sign(CurrentSpeed.Y - DecayRate.Y*DeltaTime), FMath::Sign(CurrentSpeed.Y))) {
			CurrentSpeed.Y = 0;
		} else {
			CurrentSpeed.Y -= FMath::Sign(CurrentSpeed.Y) * DecayRate.Y*DeltaTime;
		}
	}	
	if (FMath::IsNearlyZero(speedDelta.Z)) {
		if (!FMath::IsNearlyEqual(FMath::Sign(CurrentSpeed.Z - DecayRate.Z*DeltaTime), FMath::Sign(CurrentSpeed.Z))) {
			CurrentSpeed.Z = 0;
		} else {
			CurrentSpeed.Z -= FMath::Sign(CurrentSpeed.Z) * DecayRate.Z*DeltaTime;
		}
	}	
	if (FMath::IsNearlyZero(speedDelta.W)) {
		if (!FMath::IsNearlyEqual(FMath::Sign(CurrentSpeed.W - DecayRate.W*DeltaTime), FMath::Sign(CurrentSpeed.W))) {
			CurrentSpeed.W = 0;
		} else {
			CurrentSpeed.W -= FMath::Sign(CurrentSpeed.W) * DecayRate.W*DeltaTime;
		}
	}

	CurrentSpeed.X = FMath::Clamp(CurrentSpeed.X + speedDelta.X, MaxNegativeSpeed.X, MaxPositiveSpeed.X);
	CurrentSpeed.Y = FMath::Clamp(CurrentSpeed.Y + speedDelta.Y, MaxNegativeSpeed.Y, MaxPositiveSpeed.Y);
	CurrentSpeed.Z = FMath::Clamp(CurrentSpeed.Z + speedDelta.Z, MaxNegativeSpeed.Z, MaxPositiveSpeed.Z);
	CurrentSpeed.W = FMath::Clamp(CurrentSpeed.W + speedDelta.W, MaxNegativeSpeed.W, MaxPositiveSpeed.W);

	FHitResult hitResult;
	owner->AddActorLocalOffset(FVector(CurrentSpeed.X*DeltaTime, 0, 0), true, &hitResult, ETeleportType::None);
	if (hitResult.IsValidBlockingHit()) owner->AddActorLocalOffset(FVector(-CurrentSpeed.X*DeltaTime, 0, 0));
	owner->AddActorLocalOffset(FVector(0, CurrentSpeed.Y*DeltaTime, 0), true, &hitResult, ETeleportType::None);
	if (hitResult.IsValidBlockingHit()) owner->AddActorLocalOffset(FVector(0, -CurrentSpeed.Y*DeltaTime, 0));
	owner->AddActorLocalOffset(FVector(0, 0, CurrentSpeed.Z*DeltaTime), true, &hitResult, ETeleportType::None);
	if (hitResult.IsValidBlockingHit()) owner->AddActorLocalOffset(FVector(0, 0, -CurrentSpeed.Z*DeltaTime));

	owner->AddActorLocalRotation(FRotator(0.0f, CurrentSpeed.W*DeltaTime, 0.0f), true, &hitResult, ETeleportType::None);
	if (hitResult.IsValidBlockingHit()) owner->AddActorLocalRotation(FRotator(0.0f, -CurrentSpeed.W*DeltaTime, 0.0f));
}

void UCV22MovementComponent::MoveForward(float val) {
	MoveIntent.X = FMath::Clamp(val, -1.0f, 1.0f);
}
void UCV22MovementComponent::MoveRight(float val) {
	MoveIntent.Y = FMath::Clamp(val, -1.0f, 1.0f);
}
void UCV22MovementComponent::MoveUp(float val) {
	MoveIntent.Z = FMath::Clamp(val, -1.0f, 1.0f);
}
void UCV22MovementComponent::YawRight(float val) {
	MoveIntent.W = FMath::Clamp(val, -1.0f, 1.0f);
}