// Fill out your copyright notice in the Description page of Project Settings.


#include "CableGrabSegmentComponent.h"
#include "CableGrabComponent.h"

UCableGrabSegmentComponent::UCableGrabSegmentComponent() {
	SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetGenerateOverlapEvents(true);

	SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InitCapsuleSize(InitialCapsuleRadius, InitialCapsuleHalfHeight);
	this->bHiddenInGame = false;
	
}

void UCableGrabSegmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


USceneComponent* UCableGrabSegmentComponent::GetComponentToGrab_Implementation() {
	return Cast<UCableGrabComponent>(GetAttachParent());
}

void UCableGrabSegmentComponent::GrabStart_Implementation(UPrimitiveComponent * hand) {
}

void UCableGrabSegmentComponent::GrabEnd_Implementation(UPrimitiveComponent * hand) {
}

void UCableGrabSegmentComponent::GrabEvent_Implementation(UPrimitiveComponent * hand, bool buttonPressed, float xAxis, float yAxis) {
}
