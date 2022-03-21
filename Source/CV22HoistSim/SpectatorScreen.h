// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpectatorScreen.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CV22HOISTSIM_API USpectatorScreen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpectatorScreen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	UTextureRenderTarget2D* SpectatorTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	class UFont* Font = nullptr;
};
