// Fill out your copyright notice in the Description page of Project Settings.


#include "TailScanner.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "CV22.h"

// Sets default values
ATailScanner::ATailScanner()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	SetRootComponent(Root);

	VRRoot = CreateDefaultSubobject<USceneComponent>(FName("VRRoot"));
	VRRoot->SetupAttachment(Root);

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(VRRoot);

	LeftController=CreateDefaultSubobject<UHandControllerComponent>(FName("LeftHand"));
	LeftController->SetupAttachment(VRRoot);
	LeftController->SetTrackingSource(EControllerHand::Left);

	RightController = CreateDefaultSubobject<UHandControllerComponent>(FName("RightHand"));
	RightController->SetupAttachment(VRRoot);
	RightController->SetTrackingSource(EControllerHand::Right);

	LeftController->SetOtherHand(RightController);
	RightController->SetOtherHand(LeftController);
	
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(VRRoot);
}

// Called when the game starts or when spawned
void ATailScanner::BeginPlay()
{
	Super::BeginPlay();

	if (NVGMaterialBase != nullptr) {
		NVGMaterialInstance = UMaterialInstanceDynamic::Create(NVGMaterialBase, this);
		PostProcessComponent->AddOrUpdateBlendable(NVGMaterialInstance, 0.0f);
	}

	RightController->SetEnabled(false);
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	MoveToNextBeAt();
	ResetView();
}

void ATailScanner::AffectMoveToNext() {
	if (CurrentBeAtActor) {
		VRRoot->AttachToComponent(CurrentBeAtActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VRPawn"));
	} else {
		VRRoot->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VRPawn"));
	}

	LeftController->SetOtherHand(RightController);
	RightController->SetOtherHand(LeftController);
}

void ATailScanner::MoveToNextBeAt() {
	TArray<AActor*> BeAtActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, AActor::StaticClass(), FName("CanBeAt"), BeAtActors);

	bool useNextActor = false;
	for (AActor* actor : BeAtActors) {
		if (CurrentBeAtActor == nullptr) {
			CurrentBeAtActor = actor;
			AffectMoveToNext();
			return;
		}
		if (useNextActor) {
			CurrentBeAtActor = actor;
			AffectMoveToNext();
			return;
		}
		if (CurrentBeAtActor == actor) {
			useNextActor = true;
		}
	}
	if (BeAtActors.Num()) {
		CurrentBeAtActor = BeAtActors[0];
		AffectMoveToNext();
	} else {
		CurrentBeAtActor = nullptr;
		AffectMoveToNext();
	}
}


FRotator ATailScanner::GetAttachedSocketRotation() {
	if (!CurrentBeAtActor) {
		return FRotator(0, 0, 0);
	}

	TInlineComponentArray<UActorComponent*> SceneComponents;
	CurrentBeAtActor->GetComponents(USceneComponent::StaticClass(), SceneComponents, false);

	for (UActorComponent* component : SceneComponents) {
		USceneComponent* sceneComponent = Cast<USceneComponent>(component);
		if (sceneComponent) {
			if (sceneComponent->DoesSocketExist(FName("VRPawn"))) {
				return sceneComponent->GetSocketRotation(FName("VRPawn"));
			}
		}
	}

	return CurrentBeAtActor->GetActorRotation();
}


void ATailScanner::ResetView() {
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(GetAttachedSocketRotation().Yaw-VRRoot->GetComponentRotation().Yaw, EOrientPositionSelector::OrientationAndPosition);
}


void ATailScanner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATailScanner::ToggleNVGs() {
	if (NVGsOn) {
		PostProcessComponent->AddOrUpdateBlendable(NVGMaterialInstance, 0.0f);
		NVGsOn = false;
	} else {
		PostProcessComponent->AddOrUpdateBlendable(NVGMaterialInstance, 1.0f);
		NVGsOn = true;
	}
}

void ATailScanner::GrabLeft() {
	if (LeftController) LeftController->Grab();
}
void ATailScanner::ReleaseLeft() {
	if (LeftController) LeftController->ReleaseGrab();
}

void ATailScanner::GrabRight() {
	if (RightController) RightController->Grab();

}
void ATailScanner::ReleaseRight() {
	if (RightController) RightController->ReleaseGrab();
}

void ATailScanner::MoveForward(float amount) {

}
void ATailScanner::MoveRight(float amount) {

}
void ATailScanner::MoveUp(float amount) {

}
void ATailScanner::YawRight(float amount) {

}

void ATailScanner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("CyclePosition"), IE_Pressed, this, &ATailScanner::MoveToNextBeAt);
	PlayerInputComponent->BindAction(FName("ResetView"), IE_Pressed, this, &ATailScanner::ResetView);

	PlayerInputComponent->BindAction(FName("ToggleNVGs"), IE_Pressed, this, &ATailScanner::ToggleNVGs);

	PlayerInputComponent->BindAction(FName("GrabLeft"), EInputEvent::IE_Pressed, this, &ATailScanner::GrabLeft);
	PlayerInputComponent->BindAction(FName("GrabLeft"), EInputEvent::IE_Released, this, &ATailScanner::ReleaseLeft);

	PlayerInputComponent->BindAction(FName("GrabRight"), EInputEvent::IE_Pressed, this, &ATailScanner::GrabRight);
	PlayerInputComponent->BindAction(FName("GrabRight"), EInputEvent::IE_Released, this, &ATailScanner::ReleaseRight);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ATailScanner::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ATailScanner::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ATailScanner::MoveUp);
	PlayerInputComponent->BindAxis(FName("YawRight"), this, &ATailScanner::YawRight);
}

