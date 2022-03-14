// Fill out your copyright notice in the Description page of Project Settings.


#include "Hoist.h"
#include "Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "CableComponent.h"
#include "DrawDebugHelpers.h"
#include "Physics/PhysScene_PhysX.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "PhysXPublic.h"
#include "PhysicsEngine/BodyInstance.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "CableGrabComponent.h"

// Sets default values
AHoist::AHoist()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseMesh = CreateDefaultSubobject<UStaticMesh>(FName("BaseMesh"));
	HookMesh = CreateDefaultSubobject<UStaticMesh>(FName("HookMesh"));

	Base = CreateDefaultSubobject<UStaticMeshComponent>(FName("CableBase"));
	SetRootComponent(Base);

	Hook = CreateDefaultSubobject<UStaticMeshComponent>(FName("Hook"));
	Hook->SetupAttachment(Base);

	HookConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("CableBaseConstraint"));
	HookConstraint->SetupAttachment(Base);

	Cable = CreateDefaultSubobject<UCableComponent>(FName("Cable"));
	Cable->SetupAttachment(Base);
	Cable->CableLength = 1.0f;
	Cable->NumSegments = 70;
	Cable->bEnableStiffness = true;
	Cable->SolverIterations = 16;
	Cable->SubstepTime = 0.01;
	Cable->SetEnableGravity(true);
	Cable->bEnableCollision = true;
	Cable->EndLocation = FVector(0.0f);
	Cable->SetAttachEndToComponent(Hook);
	Cable->bCastFarShadow = true;
	Cable->bCastDynamicShadow = true;
	Cable->CableWidth = 4.0f;
	Cable->CollisionFriction = 0.05f;

}

// Called when the game starts or when spawned
void AHoist::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

