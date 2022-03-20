// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Grabbable.h"
#include "DrawDebugHelpers.h"
#include "Components/ShapeComponent.h"
#include "HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"


UHandControllerComponent::UHandControllerComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	HighlightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("HighlightStaticMesh"));

	SetShowDeviceModel(true);
}

void UHandControllerComponent::OnRegister() {
	Super::OnRegister();

	HighlightStaticMesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	HighlightStaticMesh->SetVisibility(false);
}

void UHandControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHighlight();

	if (GrabbedComponent) {
		IGrabbable::Execute_GrabEvent(GrabbedComponent, this, 0, 0, 0);
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

void UHandControllerComponent::UpdateHighlight() {
	if (GrabbedComponent) {
		if (HighlightedComponent) {
			HighlightedComponent->SetVisibility(true);
			HighlightStaticMesh->SetVisibility(false);
			HighlightedComponent = nullptr;
		}
		return;
	}
	UPrimitiveComponent* componentToGrab = GetComponentToGrab();
	UStaticMeshComponent* meshToHighlight = Cast<UStaticMeshComponent>(componentToGrab);

	if (HighlightedComponent) {
		if (Cast<UStaticMeshComponent>(HighlightedComponent)) {
			if (HighlightedComponent == meshToHighlight) {
				HighlightedComponent->SetVisibility(false);
				HighlightStaticMesh->SetWorldLocationAndRotation(HighlightedComponent->GetComponentLocation(), HighlightedComponent->GetComponentRotation());
				HighlightStaticMesh->SetWorldScale3D(HighlightedComponent->GetComponentScale());
				return;
			}
			if (HighlightedComponent != meshToHighlight) {
				HighlightedComponent->SetVisibility(true);
				HighlightStaticMesh->SetVisibility(false);
				HighlightedComponent = nullptr;
			}
		} else {
			UShapeComponent* shape = Cast<UShapeComponent>(HighlightedComponent);
			if (shape) {
				if (HighlightedComponent == componentToGrab) {
					HighlightedComponent->bHiddenInGame = false;
					shape->ShapeColor = FColor(0, 255, 0, 255);
					return;
				} else {
					HighlightedComponent->bHiddenInGame = true;
					HighlightedComponent = nullptr;
				}
			} else {

			}
		}
	}

	if (meshToHighlight) {
		HighlightStaticMesh->SetStaticMesh(meshToHighlight->GetStaticMesh());
		HighlightStaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		HighlightStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		if (HighlightMaterial) {
			int32 numMaterials = meshToHighlight->GetNumMaterials();
			for (int i = 0; i < numMaterials; i++) {
				HighlightStaticMesh->SetMaterial(i, HighlightMaterial);
			}
		} else {
			UE_LOG(LogTemp, Warning, TEXT("No hightlight material for UHandControllerComponent"))
		}

		HighlightedComponent = meshToHighlight;
		HighlightedComponent->SetVisibility(false);
		HighlightStaticMesh->SetVisibility(true);
		HighlightStaticMesh->SetWorldLocationAndRotation(meshToHighlight->GetComponentLocation(), meshToHighlight->GetComponentRotation());
		HighlightStaticMesh->SetWorldScale3D(meshToHighlight->GetComponentScale());
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

	bool result = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), handLocation, GrabDistance*GetComponentScale().GetMin(), objectTypes, nullptr, actorsToIgnore, outComponents);
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