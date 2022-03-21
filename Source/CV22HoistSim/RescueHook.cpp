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

}

void URescueHook::OnRegister() {
	Super::OnRegister();
	HoistOwner = Cast<UHoistComponent>(GetAttachParent());
}

void URescueHook::BeginPlay() {
	Super::BeginPlay();
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


UPrimitiveComponent * URescueHook::GetAttachedDevice() {
	return nullptr;
}
