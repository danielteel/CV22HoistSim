// Fill out your copyright notice in the Description page of Project Settings.


#include "CV22.h"
#include "Components/StaticMeshComponent.h"
#include "HoistControlPanel.h"
#include "HoistComponent.h"
#include "TailScanner.h"
#include "RescueHook.h"
#include "RotorComponent.h"
#include "HandControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "CV22MovementComponent.h"
#include "CV22HoistSimGameModeBase.h"

// Sets default values
ACV22::ACV22()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(Root);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(FName("Body"));
	Body->SetupAttachment(Root);
	Body->SetMassOverrideInKg(NAME_None, 18143.0f, true);

	HoistComponent = CreateDefaultSubobject<UHoistComponent>(FName("Hoist"));
	HoistComponent->SetupAttachment(Body, FName("Hoist"));

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
	AircraftNoise->AttenuationOverrides.AttenuationShapeExtents = FVector(4000.0f, 0.0f, 0.0f);
	AircraftNoise->AttenuationOverrides.FalloffDistance = 60000.0f;
	AircraftNoise->AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::NaturalSound;
	AircraftNoise->AttenuationOverrides.dBAttenuationAtMax = -29.0f;
	AircraftNoise->AttenuationOverrides.bEnableOcclusion = true;
	AircraftNoise->AttenuationOverrides.OcclusionLowPassFilterFrequency = 2500.0f;

	HoistControlPanel = CreateDefaultSubobject<UHoistControlPanel>(FName("HoistControlPanel"));
	HoistControlPanel->SetupAttachment(Body, "HCP");
	
	MovementComponent = CreateDefaultSubobject<UCV22MovementComponent>(FName("MovementComponent"));
}

// Called when the game starts or when spawned
void ACV22::BeginPlay()
{
	Super::BeginPlay();
	RotorLeft->IsLeftRotor = true;
	Body->SetSimulatePhysics(true);
	Body->SetUseCCD(true);
	MovementComponent->Setup(Body);
}


void ACV22::UpdateRotorDust(UParticleSystemComponent* dust, FVector rotorPosition) {

	ACV22HoistSimGameModeBase* gameMode = Cast<ACV22HoistSimGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) return;

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
		float scale = FMath::Clamp(1.5f - (hitResult.Distance / 6500), 0.7f, 1.3f);
		dust->SetWorldLocation(hitResult.ImpactPoint+FVector::UpVector*150*scale);
		dust->SetWorldScale3D(FVector(scale));
		float dustSpawnRate = gameMode->DustFactor;
		dust->SetFloatParameter("Amount", dustSpawnRate);
	} else {
		dust->Deactivate();
	}
}
USceneComponent* ACV22::GetComponentToAttachTo_Implementation() {
	return Body;
}

void ACV22::EnteredActor_Implementation(APawn* pawn) {
	TailScannerInsideMe = Cast<ATailScanner>(pawn);
}

void ACV22::LeftActor_Implementation(APawn* pawn) {
	UHoistComponent* hoist = Cast<UHoistComponent>(GetComponentByClass(UHoistComponent::StaticClass()));
	if (!hoist) return;

	UHandControllerComponent* leftController = TailScannerInsideMe->GetLeftController();
	UHandControllerComponent* rightController = TailScannerInsideMe->GetRightController();
	if (leftController) {
		if (hoist->IsAHoistGrabbableComponent(leftController->GetGrabbedComponent())) leftController->ReleaseGrab();
	}
	if (rightController) {
		if (hoist->IsAHoistGrabbableComponent(rightController->GetGrabbedComponent())) rightController->ReleaseGrab();
	}
	TailScannerInsideMe = nullptr;
}

// Called every frame
void ACV22::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Root->SetWorldLocation(Body->GetComponentLocation());
	FRotator bodyYawRotation = Body->GetComponentRotation();
	bodyYawRotation.Pitch = 0;
	bodyYawRotation.Roll = 0;
	Root->SetWorldRotation(bodyYawRotation);

	UpdateRotorDust(DustLeft, RotorLeft->GetComponentLocation());
	UpdateRotorDust(DustRight, RotorRight->GetComponentLocation());

	UHoistControlPanel* hcp = Cast<UHoistControlPanel>(GetComponentByClass(UHoistControlPanel::StaticClass()));
	if (hcp) {
		bool wasMovedToHand = false;
		if (TailScannerInsideMe) {
			UHandControllerComponent* rightController = TailScannerInsideMe->GetRightController();
			if (rightController) {
				wasMovedToHand = true;
				FVector newHCPLocation = rightController->GetComponentLocation();
				hcp->SetWorldLocation(newHCPLocation);
				hcp->SetWorldRotation(rightController->GetComponentRotation());
				hcp->AddLocalRotation(HandControllerOffset);
				FVector offset = FVector(-4.5f, 5.5f, -16.0f);
				hcp->AddLocalOffset(offset);
			}
		}
		if (wasMovedToHand == false) {
			FVector socketLoc;
			FRotator socketRot;
			Body->GetSocketWorldLocationAndRotation(FName("HCP"), socketLoc, socketRot);
			hcp->SetWorldLocationAndRotation(socketLoc, socketRot);
		}
	}
}
