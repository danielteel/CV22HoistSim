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

	FRotator acRotation = Aircraft->GetRelativeRotation();
	FVector localVelocity = GetOwner()->GetActorRotation().UnrotateVector(Aircraft->GetComponentVelocity());


	FVector accel = (LastVelocity - localVelocity) / DeltaTime;
	FVector hoverForce = GetOwner()->GetActorUpVector() * Aircraft->GetMass() * (-GetWorld()->GetGravityZ());

	Aircraft->AddForce(hoverForce);
	Aircraft->AddForce(hoverForce*MoveIntent.Z);

	Aircraft->AddForce(10000000.0f * MoveIntent.X * GetOwner()->GetActorForwardVector());
	Aircraft->AddForce(10000000.0f * MoveIntent.Y * GetOwner()->GetActorRightVector());

	float maxPitchRoll = 15.0f;
	float desiredPitch = FMath::Clamp(accel.X/1000.0f, -1.0f, 1.0f) * maxPitchRoll;
	float desiredRoll = FMath::Clamp(accel.Y/1000.0f, -1.0f, 1.0f) * -maxPitchRoll;

	float pitchDiff = desiredPitch - acRotation.Pitch;
	float rollDiff = desiredRoll - acRotation.Roll;
	
	FVector rotVel = Aircraft->GetUpVector() * 30.0f * DeltaTime * MoveIntent.W;
	rotVel += Aircraft->GetForwardVector() * 60.f * DeltaTime * (-rollDiff/30.f);
	rotVel += Aircraft->GetRightVector() * 60.0f * DeltaTime * (-pitchDiff/30.f);

	Aircraft->SetPhysicsAngularVelocityInDegrees(rotVel, true);
	   	 
	LastVelocity = localVelocity;
}

void UCV22MovementComponent::Setup(UPrimitiveComponent* aircraft) {
	Aircraft = aircraft;
	Aircraft->SetPhysicsMaxAngularVelocity(30);
	Aircraft->SetLinearDamping(0.2f);
	Aircraft->SetAngularDamping(1.0f);
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