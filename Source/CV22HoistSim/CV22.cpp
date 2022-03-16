// Fill out your copyright notice in the Description page of Project Settings.


#include "CV22.h"
#include "Components/StaticMeshComponent.h"
#include "HoistControlPanel.h"
#include "HoistComponent.h"
#include "TailScanner.h"
#include "RotorComponent.h"
#include "HandControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "CV22MovementComponent.h"

// Sets default values
ACV22::ACV22()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(Root);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(FName("Body"));
	Body->SetupAttachment(Root);

	RotorLeft = CreateDefaultSubobject<URotorComponent>(FName("RotorLeft"));
	RotorLeft->SetupAttachment(Body, FName("RotorLeft"));

	RotorRight = CreateDefaultSubobject<URotorComponent>(FName("RotorRight"));
	RotorRight->SetupAttachment(Body, FName("RotorRight"));

	
	DustLeft = CreateDefaultSubobject<UParticleSystemComponent>(FName("DustLeft"));
	DustLeft->SetupAttachment(RotorLeft);
	DustLeft->bAutoActivate = false;
	
	DustRight = CreateDefaultSubobject<UParticleSystemComponent>(FName("DustRight"));
	DustRight->SetupAttachment(RotorRight);
	DustRight->bAutoActivate = false;

	AircraftNoise = CreateDefaultSubobject<UAudioComponent>(FName("AircraftNoise"));
	AircraftNoise->SetupAttachment(Body);
	AircraftNoise->bOverrideAttenuation = true;
	AircraftNoise->bAllowSpatialization = true;
	AircraftNoise->AttenuationOverrides.AttenuationShapeExtents = FVector(3000, 0, 0);
	AircraftNoise->AttenuationOverrides.FalloffDistance = 20000;
	
	MovementComponent = CreateDefaultSubobject<UCV22MovementComponent>(FName("MovementComponent"));


	Tags.Add(FName("CanBeAt"));
}

// Called when the game starts or when spawned
void ACV22::BeginPlay()
{
	Super::BeginPlay();
	RotorLeft->IsLeftRotor = true;
	MovementComponent->Setup(Body);
	Body->SetSimulatePhysics(true);
}


void ACV22::UpdateRotorDust(UParticleSystemComponent* dust, FVector rotorPosition) {
	FHitResult hitResult;
	FVector traceStart = rotorPosition;
	FVector traceEnd = traceStart  + -Body->GetUpVector() * 6500.0f;

	FCollisionShape shape = FCollisionShape::MakeSphere(400.0f);

	FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
	queryParams.AddIgnoredActor(this);
	queryParams.bTraceComplex = true;
	queryParams.bReturnPhysicalMaterial = true;

	FCollisionResponseParams responseParams = FCollisionResponseParams::DefaultResponseParam;
	bool sweepSuccess = GetWorld()->SweepSingleByChannel(hitResult, traceStart, traceEnd, FRotator(0, 0, 0).Quaternion(), ECollisionChannel::ECC_PhysicsBody, shape, queryParams, responseParams);

	if (sweepSuccess && hitResult.PhysMaterial!=nullptr && hitResult.PhysMaterial->SurfaceType == EPhysicalSurface::SurfaceType1) {
		if (!dust->IsActive()) dust->Activate();
		dust->SetWorldLocation(hitResult.ImpactPoint);
		float dustSpawnRate = FMath::Clamp(1.0f-((hitResult.Distance-2048.0f) / 4452.0f), 0.0f, 1.0f);
		dust->SetFloatParameter("Amount", dustSpawnRate);
	} else {
		dust->Deactivate();
	}
}

// Called every frame
void ACV22::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateRotorDust(DustLeft, RotorLeft->GetComponentLocation());
	UpdateRotorDust(DustRight, RotorRight->GetComponentLocation());
}
