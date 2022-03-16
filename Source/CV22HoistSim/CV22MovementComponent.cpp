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

	if (!Aircraft) {
		UE_LOG(LogTemp, Error, TEXT("No aircraft set in cv22movementcomponent"));
		return;
	}

	Aircraft->AddForce(FVector::UpVector * Aircraft->GetMass() * (-GetWorld()->GetGravityZ()));


}

void UCV22MovementComponent::Setup(UPrimitiveComponent* aircraft) {
	Aircraft = aircraft;
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