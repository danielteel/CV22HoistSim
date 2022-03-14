// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Grabbable.h"
#include "DrawDebugHelpers.h"


UHandControllerComponent::UHandControllerComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	SetShowDeviceModel(true);
}
void UHandControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

void UHandControllerComponent::Grab() {
	if (!Enabled) return;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> actorsToIgnore;
	TArray<UPrimitiveComponent*> outComponents;

	FVector HandLocation = GetComponentLocation();
	bool result = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), HandLocation, GrabDistance, objectTypes, nullptr, actorsToIgnore, outComponents);
	if (result) {

		UPrimitiveComponent* componentToGrab = nullptr;
		float distanceToGrab = 0.0f;
		for (int i = 0; i < outComponents.Num(); i++) {
			IGrabbable* grabbableObject = Cast<IGrabbable>(outComponents[i]);
			if (grabbableObject) {

				float distToObj = FVector::Dist(HandLocation, outComponents[i]->GetComponentLocation());
				if (distToObj < distanceToGrab || componentToGrab == nullptr) {
					componentToGrab = outComponents[i];
					distanceToGrab = distToObj;
				}
			}
		}

		if (componentToGrab) {
			if (OtherHand) {
				if (OtherHand->GrabbedComponent == componentToGrab) OtherHand->ReleaseGrab();
			}
			if (IGrabbable::Execute_GrabStart(componentToGrab, this)) {
				GrabbedComponent = componentToGrab;
			}
		}
	}
}

void UHandControllerComponent::ReleaseGrab() {
	if (GrabbedComponent) {
		IGrabbable::Execute_GrabEnd(GrabbedComponent, this);
		GrabbedComponent = nullptr;
	}
}