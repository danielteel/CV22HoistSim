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


void ULever::LeverSetToPitch(float pitch) {

}