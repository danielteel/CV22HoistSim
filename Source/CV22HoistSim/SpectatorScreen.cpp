// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorScreen.h"
#include "HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "CanvasItem.h"
#include "Kismet/GameplayStatics.h"
#include "CV22MovementComponent.h"
#include "Engine/Font.h"
#include "TailScanner.h"

// Sets default values for this component's properties
USpectatorScreen::USpectatorScreen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.03f;
}


// Called when the game starts
void USpectatorScreen::BeginPlay()
{
	Super::BeginPlay();

	if (SpectatorTexture != nullptr) {
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenTexture(SpectatorTexture);
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenModeTexturePlusEyeLayout(FVector2D(0.0f, 0.0f), FVector2D(0.35f, 0.5f), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f), false, false, false);
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenMode(ESpectatorScreenMode::TexturePlusEye);
	} else {
		UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenMode(ESpectatorScreenMode::SingleEyeCroppedToFill);
	}
	
}


// Called every frame
void USpectatorScreen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ATailScanner* tailScanner = Cast<ATailScanner>(GetOwner());
	if (tailScanner) {
		AActor* currentBeAtActor = tailScanner->GetCurrentBeAtActor();

		USceneCaptureComponent2D* captureComponent = Cast<USceneCaptureComponent2D>(currentBeAtActor->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
		if (!captureComponent)  captureComponent = Cast<USceneCaptureComponent2D>(tailScanner->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
		if (captureComponent) captureComponent->CaptureScene();
		
		UCV22MovementComponent* movementComponent = Cast<UCV22MovementComponent>(currentBeAtActor->GetComponentByClass(UCV22MovementComponent::StaticClass()));

		if (movementComponent) {
			FVector velocity = movementComponent->GetVelocity();
			UCanvas* canvas = nullptr;
			FVector2D size;
			FDrawToRenderTargetContext context;
			UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), SpectatorTexture, canvas, size, context);

			if (canvas) {
				FVector2D center = FVector2D(0.5f, 0.5f)*size;
				float minComp = size.GetMin();
				canvas->K2_DrawBox(center -FVector2D(0.25f, 0.25f)*minComp, FVector2D(0.5f, 0.5f)*minComp, 3.0f, FLinearColor::Green);
				canvas->K2_DrawLine(FVector2D(0.5f, 0.5f)*size, FVector2D(center.X + ((velocity.Y / 257.222f)*0.25f)*minComp, center.Y - ((velocity.X / 257.222f)*0.25f)*minComp), 2.0f, FLinearColor::Green);
				if (Font) {
					canvas->K2_DrawText(Font, FString("RADALT: ") + FString::FromInt(movementComponent->GetAGL()), FVector2D(0.5f, 0.76f)*size, FVector2D(2.f, 2.f), FLinearColor::White, 0.0f, FLinearColor::Black, FVector2D::UnitVector, true, false, true, FLinearColor::Black);
				}
				UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), context);
			}
		}
	}
}

