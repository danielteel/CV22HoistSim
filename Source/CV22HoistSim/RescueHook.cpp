// Fill out your copyright notice in the Description page of Project Settings.


#include "RescueHook.h"
#include "Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Physics/PhysScene_PhysX.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "PhysXPublic.h"
#include "PhysicsEngine/BodyInstance.h"
#include "GameFramework/PhysicsVolume.h"
#include "HoistComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

URescueHook::URescueHook() {
	PrimaryComponentTick.bCanEverTick = true;


	HookConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("HookConstraint"));
}

void URescueHook::OnRegister() {
	Super::OnRegister();
	if (HookConstraint) {
		HookConstraint->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	HoistOwner = Cast<UHoistComponent>(GetAttachParent());
}

void URescueHook::BeginPlay() {
	Super::BeginPlay();
	HookConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	HookConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 2.0f);
	HookConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 2.0f);
	HookConstraint->ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = 1;
	HookConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Damping = 800.0f;
	HookConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Stiffness = 10.0f;
}

void URescueHook::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsSimulatingPhysics() && !RigidBodyIsAwake()) WakeRigidBody();

	Velocity = (GetComponentLocation() - LastLocation)/DeltaTime;

	LastLocation = GetComponentLocation();
}

USceneComponent* URescueHook::GetComponentToGrab_Implementation() {
	return this;
}
void URescueHook::GrabStart_Implementation(UPrimitiveComponent * hand) {
	SetSimulatePhysics(false);
	this->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttachToComponent(hand, FAttachmentTransformRules::KeepWorldTransform);
}
void URescueHook::GrabEnd_Implementation(UPrimitiveComponent * hand) {
	FVector lastVelocity = GetComponentVelocity();
	DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	this->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetSimulatePhysics(true);	

	if (HoistOwner) HoistOwner->GrabEndRescueHook();
	
	this->SetPhysicsLinearVelocity(Velocity);
}
void URescueHook::GrabEvent_Implementation(UPrimitiveComponent * hand, bool buttonPressed, float xAxis, float yAxis) {
}


void URescueHook::Attach(UPrimitiveComponent* device) {
	AttachedDevice = device;
	FRotator deviceRot = device->GetComponentRotation();;
	SetWorldRotation(deviceRot, false, nullptr, ETeleportType::ResetPhysics);
	FVector deviceSocketLocation = device->GetSocketLocation(FName("Hook"));
	FVector hookSocketLocation = GetSocketLocation(FName("Hook"));
	device->AddWorldOffset(hookSocketLocation - deviceSocketLocation, false, nullptr, ETeleportType::ResetPhysics);
	device->SetPhysicsAngularVelocity(FVector(0.0f));
	device->SetPhysicsLinearVelocity(FVector(0.0f));
	SetPhysicsLinearVelocity(FVector(0.0f));
	SetPhysicsAngularVelocity(FVector(0.0f));
	HookConstraint->BreakConstraint();
	HookConstraint->SetWorldLocation(hookSocketLocation);
	HookConstraint->SetConstrainedComponents(this, NAME_None, device, NAME_None);
}

UPrimitiveComponent * URescueHook::GetAttachedDevice() {
	return AttachedDevice;
}

void URescueHook::Dettach() {
	AttachedDevice = nullptr;
	HookConstraint->BreakConstraint();
}