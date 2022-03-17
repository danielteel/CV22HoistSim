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
	float yaw = GetOwner()->GetActorRotation().Yaw;
	FVector CurrentVelocity = Aircraft->GetComponentVelocity();
	FVector LocalVelocity = GetOwner()->GetActorRotation().UnrotateVector(CurrentVelocity);


	FVector Accel = (LastVelocity - LocalVelocity) / DeltaTime;
	FVector hoverForce = GetOwner()->GetActorUpVector() * Aircraft->GetMass() * (-GetWorld()->GetGravityZ());

	Aircraft->AddForce(hoverForce);
	Aircraft->AddForce(hoverForce*MoveIntent.Z);

	Aircraft->AddForce(10000000.0f * MoveIntent.X * GetOwner()->GetActorForwardVector());
	Aircraft->AddForce(10000000.0f * MoveIntent.Y * GetOwner()->GetActorRightVector());

	float maxPitchRoll = 15.0f;
	float desiredPitch = FMath::Clamp(Accel.X/1000.0f, -1.0f, 1.0f) * maxPitchRoll;
	float desiredRoll = FMath::Clamp(Accel.Y/1000.0f, -1.0f, 1.0f) * -maxPitchRoll;

	float pitchDiff = desiredPitch - acRotation.Pitch;
	float rollDiff = desiredRoll - acRotation.Roll;
	
	Roll = Roll + rollDiff * DeltaTime;
	Pitch = Pitch + pitchDiff * DeltaTime;
	yaw += MoveIntent.W*DeltaTime*30.0f;
	Aircraft->SetWorldRotation(FRotator(Pitch, yaw, Roll), true);
	   	 
	LastVelocity = LocalVelocity;
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