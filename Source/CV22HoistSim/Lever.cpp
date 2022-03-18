// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"
#include "LeverHandle.h"

// Sets default values for this component's properties
ULever::ULever()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Base"));
	BaseMeshComponent->SetupAttachment(this);
	

	LeverHandleComponent = CreateDefaultSubobject<ULeverHandle>(FName("Handle"));
	LeverHandleComponent->SetupAttachment(BaseMeshComponent, FName("Lever"));
	LeverHandleComponent->Setup(MinPitch, MaxPitch, 0.0f);
}


// Called when the game starts
void ULever::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ULever::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULever::SetLeverValue(int value) {
	value = FMath::Clamp(value, 0, Positions);
	float leverAlpha = float(value) / (float(Positions) - 1);
	CurrentValue = value;
	LeverHandleComponent->SetValue(leverAlpha);
	OnLeverWasChanged.Broadcast(CurrentValue);
}

void ULever::LeverWasSet(float value) {
	float delta = MaxPitch - MinPitch;
	float divider = delta / float(Positions);
	int step = FMath::Floor((value * delta) / Positions);
	float leverAlpha = float(step) / (float(Positions) - 1);
	CurrentValue = step;
	LeverHandleComponent->SetValue(leverAlpha);

	OnLeverWasChanged.Broadcast(CurrentValue);
}