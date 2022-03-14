// Fill out your copyright notice in the Description page of Project Settings.


#include "CableGrabComponent.h"
#include "CableComponent.h"
#include "CableGrabSegmentComponent.h"
#include "Components/PrimitiveComponent.h"
#include "UObject/UObjectGlobals.h"
#include "DrawDebugHelpers.h"

UCableGrabComponent::UCableGrabComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCableGrabComponent::BeginPlay() {
	Super::BeginPlay();
}


// Called every frame
void UCableGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (MasterCable) {
		TArray<FVector> cableParticleLocations;
		MasterCable->GetCableParticleLocations(cableParticleLocations);
		int32 segments = FMath::Min(NumCableSegments, cableParticleLocations.Num() - 1);
		if (segments > 0) {
			for (int i = 0; i < segments; i++) {
				float length = FVector::Dist(cableParticleLocations[i], cableParticleLocations[i + 1]) / 2.0f;
				FRotator segmentRotation = (cableParticleLocations[i] - cableParticleLocations[i + 1]).GetSafeNormal().Rotation();
				segmentRotation.Pitch += 90.0f;
				CableSegments[i]->SetWorldLocation((cableParticleLocations[i] + cableParticleLocations[i + 1]) / 2.0f);
				CableSegments[i]->SetWorldRotation(segmentRotation);
				CableSegments[i]->SetCapsuleSize(MasterCable->CableWidth, length);
			}
		}
	}
}


UCableGrabSegmentComponent* UCableGrabComponent::NewGrabSegment() {
	UCableGrabSegmentComponent* Component = NewObject<UCableGrabSegmentComponent>(this, UCableGrabSegmentComponent::StaticClass());
	Component->SetupAttachment(this);
	Component->RegisterComponent();
	return Component;
}

void UCableGrabComponent::Setup(class UCableComponent* cable) {
	/*
	for (UCableGrabSegmentComponent* segment : CableSegments) {
		if (segment) segment->DestroyComponent();
	}
	CableSegments.SetNum(0, true);
	*/
	MasterCable = cable;
	if (cable) {
		NumCableSegments = cable->NumSegments;
		for (int i = 0; i < NumCableSegments; i++) {
			CableSegments.Add(NewGrabSegment());
		}
	}
}

bool UCableGrabComponent::IsGrabbed() {
	return Grabbed;
}

void UCableGrabComponent::Grab(UPrimitiveComponent* hand) {
	Grabbed = true;
	GrabbedHand = hand;
}

void UCableGrabComponent::Release() {
	Grabbed = false;
	GrabbedHand = nullptr;
}
