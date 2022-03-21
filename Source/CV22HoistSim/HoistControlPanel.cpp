// Fill out your copyright notice in the Description page of Project Settings.


#include "HoistControlPanel.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"
#include "HoistComponent.h"


// Sets default values for this component's properties
UHoistControlPanel::UHoistControlPanel() {
	PrimaryComponentTick.bCanEverTick = true;


}

void UHoistControlPanel::OnRegister() {
	Super::OnRegister();
	PowerSwitchMeshComponent = NewObject<UStaticMeshComponent>(this, FName("PowerSwitch"));
	PowerSwitchMeshComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("OnOff"));
	PowerSwitchMeshComponent->RegisterComponent();
	PowerSwitchMeshComponent->SetStaticMesh(PowerSwitchMesh);

	UpDownMeshComponent = NewObject<UStaticMeshComponent>(this, FName("UpDown"));
	UpDownMeshComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("UpDown"));
	UpDownMeshComponent->RegisterComponent();
	UpDownMeshComponent->SetStaticMesh(UpDownMesh);

	JettisonMeshComponent = NewObject<UStaticMeshComponent>(this, FName("Jettison"));
	JettisonMeshComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Jettison"));
	JettisonMeshComponent->RegisterComponent();
	JettisonMeshComponent->SetStaticMesh(JettisonMesh);
}

// Called when the game starts
void UHoistControlPanel::BeginPlay() {
	Super::BeginPlay();

	PowerSwitchMeshComponent->SetVisibility(false);
}


UHoistComponent* UHoistControlPanel::GetHoistComponent() {
	AActor* owner = GetOwner();
	if (owner) {
		return owner->FindComponentByClass<UHoistComponent>();
	}
	return nullptr;
}

// Called every frame
void UHoistControlPanel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UHoistComponent* hoist = GetHoistComponent();

	//Check to show power light
	PowerSwitchMeshComponent->SetVisibility(PowerIsOn);

	//Update hoist up/down switch
	float upDownPitch = -60.0f*ExtendCommand;
	FRotator upDownRotation = FRotator(0.0f, 0.0f, upDownPitch);
	UpDownMeshComponent->SetRelativeRotation(upDownRotation);

	//Jettison Switch
	if (JettisonState) {
		JettisonMeshComponent->SetRelativeRotation(FRotator(90, 0, 0));
	}else {
		JettisonMeshComponent->SetRelativeRotation(FRotator(0, 0, 0));
	}

	if (hoist) {
		//Send extend command only if power is on
		if (PowerIsOn) {
			hoist->Extend(ExtendCommand);
		} else {
			hoist->Extend(0.0f);
		}

		//Set jettison switch state
		if (JettisonState) hoist->JettisonHoist();
	}
}

void UHoistControlPanel::SetPowerState(bool powerState) {
	PowerIsOn = powerState;
}

bool UHoistControlPanel::GetPowerState() {
	return PowerIsOn;
}

void UHoistControlPanel::SetExtendCommand(float extend) {
	ExtendCommand = FMath::Clamp(extend, -1.0f, 1.0f);
}

void UHoistControlPanel::SetJettison(bool jettison) {
	JettisonState = jettison;
	UHoistComponent* hoist = GetHoistComponent();
	if (hoist && jettison) hoist->JettisonHoist();
}

