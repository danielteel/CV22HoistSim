// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Grabbable.h"
#include "DrawDebugHelpers.h"


UHandControllerComponent::UHandControllerComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	HighlightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("HighlightStaticMesh"));
	HighlightStaticMesh->SetupAttachment(this);
	HighlightStaticMesh->SetVisibility(false);

	SetShowDeviceModel(true);
}
void UHandControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GrabbedComponent) {
		IGrabbable::Execute_GrabEvent(GrabbedComponent, this, 0, 0, 0);
	} else {
		HighlightGrabbable();
	}
}

void UHandControllerComponent::SetOtherHand(UHandControllerComponent* Hand) {
	OtherHand = Hand;
}

void UHandControllerComponent::SetEnabled(bool enabled) {
	Enabled = enabled;
	if (enabled == false) {
		ReleaseGrab();
	}
}

void UHandControllerComponent::HighlightGrabbable() {
	UStaticMeshComponent* componentToHighlight = Cast<UStaticMeshComponent>(GetComponentToGrab());

	if (componentToHighlight) {
		HighlightStaticMesh->SetStaticMesh(componentToHighlight->GetStaticMesh());
		HighlightStaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		HighlightStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		int32 numMaterials = componentToHighlight->GetNumMaterials();
		for (int i = 0; i < numMaterials; i++) {
			HighlightStaticMesh->SetMaterial(i, HighlightMaterial);
		}

		HighlightStaticMesh->SetVisibility(true);
		HighlightStaticMesh->SetWorldLocationAndRotation(componentToHighlight->GetComponentLocation(), componentToHighlight->GetComponentRotation());
		HighlightStaticMesh->SetWorldScale3D(componentToHighlight->GetComponentScale()+FVector(0.05f,0.05f,0.05f));
	} else {
		HighlightStaticMesh->SetVisibility(false);
	}
}

UPrimitiveComponent* UHandControllerComponent::GetComponentToGrab() {
	if (!Enabled) return nullptr;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> actorsToIgnore;
	TArray<UPrimitiveComponent*> outComponents;

	UPrimitiveComponent* componentToGrab = nullptr;

	FVector handLocation = GetComponentLocation();

	bool result = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), handLocation, GrabDistance, objectTypes, nullptr, actorsToIgnore, outComponents);
	if (result) {
		float distanceToGrab = 0.0f;
		for (int i = 0; i < outComponents.Num(); i++) {
			IGrabbable* grabbableObject = Cast<IGrabbable>(outComponents[i]);
			if (grabbableObject) {

				float distToObj = FVector::Dist(handLocation, outComponents[i]->GetComponentLocation());
				if (distToObj < distanceToGrab || componentToGrab == nullptr) {
					componentToGrab = outComponents[i];
					distanceToGrab = distToObj;
				}
			}
		}
	}
	return componentToGrab;
}


void UHandControllerComponent::Grab() {
	if (!Enabled) return;
	
	USceneComponent* componentToGrab = GetComponentToGrab();
	if (componentToGrab) {
		componentToGrab = IGrabbable::Execute_GetComponentToGrab(componentToGrab);

		HighlightStaticMesh->SetVisibility(false);
		if (OtherHand) {
			if (OtherHand->GrabbedComponent == componentToGrab) OtherHand->ReleaseGrab();
		}
		IGrabbable::Execute_GrabStart(componentToGrab, this);
		GrabbedComponent = componentToGrab;
	}
}

void UHandControllerComponent::ReleaseGrab() {
	if (GrabbedComponent) {
		IGrabbable::Execute_GrabEnd(GrabbedComponent, this);
		GrabbedComponent = nullptr;
	}
}