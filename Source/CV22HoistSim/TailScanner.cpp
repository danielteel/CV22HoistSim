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
#include "CanBeAt.h"
#include "CanBeAtMovementComponent.h"
#include "HoistControlPanel.h"

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
	MoveToNextBeAt();
	ResetView();

	UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	if (SpectatorTexture != nullptr) {
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenTexture(SpectatorTexture);
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenModeTexturePlusEyeLayout(FVector2D(0.03f, 0.03f), FVector2D(0.35f, 0.35f), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f), false, false, false);
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenMode(ESpectatorScreenMode::TexturePlusEye);
	} else {
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenMode(ESpectatorScreenMode::SingleEyeCroppedToFill);
	}
}

void ATailScanner::AffectMoveToNext(AActor* actorToBeAt) {
	if (CurrentBeAtActor) {
		ICanBeAt::Execute_LeftActor(CurrentBeAtActor, this);
	}
	CurrentBeAtActor = actorToBeAt;
	if (CurrentBeAtActor) {
		ICanBeAt::Execute_EnteredActor(CurrentBeAtActor, this);
		CurrentBeAtComponent = ICanBeAt::Execute_GetComponentToAttachTo(CurrentBeAtActor);
		if (!CurrentBeAtComponent) CurrentBeAtComponent = CurrentBeAtActor->GetRootComponent();
		VRRoot->AttachToComponent(CurrentBeAtComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VRPawn"));
	} else {
		VRRoot->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("VRPawn"));
	}

	LeftController->SetOtherHand(RightController);
	RightController->SetOtherHand(LeftController);
}

void ATailScanner::MoveToNextBeAt() {
	TArray<AActor*> beAtActors;
	UGameplayStatics::GetAllActorsWithInterface(this, UCanBeAt::StaticClass(), beAtActors);

	bool useNextActor = false;
	for (AActor* actor : beAtActors) {
		if (CurrentBeAtActor == nullptr) {
			AffectMoveToNext(actor);
			return;
		}
		if (useNextActor) {
			AffectMoveToNext(actor);
			return;
		}
		if (CurrentBeAtActor == actor) {
			useNextActor = true;
		}
	}
	if (beAtActors.Num()) {
		AffectMoveToNext(beAtActors[0]);
	} else {
		AffectMoveToNext(nullptr);
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
	
	if (CurrentBeAtActor) {
		UHoistControlPanel* hcp = Cast<UHoistControlPanel>(CurrentBeAtActor->GetComponentByClass(UHoistControlPanel::StaticClass()));
		if (hcp) {
			hcp->SetExtendCommand(HoistExtendIntent);
		}

		HoistExtendIntent = 0;
	}
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

UCanBeAtMovementComponent* ATailScanner::GetCanBeAtMovementComponent() {
	UCanBeAtMovementComponent* movementComponent = Cast<UCanBeAtMovementComponent>(CurrentBeAtActor->GetComponentByClass(UCanBeAtMovementComponent::StaticClass()));
	return movementComponent;
}

void ATailScanner::MoveForward(float amount) {
	UCanBeAtMovementComponent* movementComponent = GetCanBeAtMovementComponent();
	if (!movementComponent) return;
	movementComponent->MoveForward(amount);
}
void ATailScanner::MoveRight(float amount) {
	UCanBeAtMovementComponent* movementComponent = GetCanBeAtMovementComponent();
	if (!movementComponent) return;
	movementComponent->MoveRight(amount);
}
void ATailScanner::MoveUp(float amount) {
	UCanBeAtMovementComponent* movementComponent = GetCanBeAtMovementComponent();
	if (!movementComponent) return;
	movementComponent->MoveUp(amount);
}
void ATailScanner::YawRight(float amount) {
	UCanBeAtMovementComponent* movementComponent = GetCanBeAtMovementComponent();
	if (!movementComponent) return;
	movementComponent->YawRight(amount);
}


void ATailScanner::HoistSetPower(bool on) {
	if (!CurrentBeAtActor) return;
	UHoistControlPanel* hcp = Cast<UHoistControlPanel>(CurrentBeAtActor->GetComponentByClass(UHoistControlPanel::StaticClass()));
	if (hcp) {
		hcp->SetPowerState(on);
	}
}
void ATailScanner::HoistSetJettison(bool state) {
	if (!CurrentBeAtActor) return;
	UHoistControlPanel* hcp = Cast<UHoistControlPanel>(CurrentBeAtActor->GetComponentByClass(UHoistControlPanel::StaticClass()));
	if (hcp) {
		hcp->SetJettison(state);
	}
}
void ATailScanner::HoistSetUpDown(float amount) {
	HoistExtendIntent += amount;
}

void ATailScanner::HoistPower() {//Toggled via whatever input
	if (!CurrentBeAtActor) return;
	UHoistControlPanel* hcp = Cast<UHoistControlPanel>(CurrentBeAtActor->GetComponentByClass(UHoistControlPanel::StaticClass()));
	if (hcp) {
		hcp->SetPowerState(!hcp->GetPowerState());
		UE_LOG(LogTemp, Warning, TEXT("IM CALLED"))
	}
}
void ATailScanner::HoistJettison() {//Toggled via whatever input
	HoistSetJettison(true);
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

	PlayerInputComponent->BindAction(FName("HoistPower"), EInputEvent::IE_Pressed, this, &ATailScanner::HoistPower);
	PlayerInputComponent->BindAction(FName("HoistJettison"), EInputEvent::IE_Pressed, this, &ATailScanner::HoistJettison);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ATailScanner::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ATailScanner::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ATailScanner::MoveUp);
	PlayerInputComponent->BindAxis(FName("YawRight"), this, &ATailScanner::YawRight);

	PlayerInputComponent->BindAxis(FName("HoistUpDown"), this, &ATailScanner::HoistSetUpDown);
	
}

