#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "HoistControlPanel.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CV22HOISTSIM_API UHoistControlPanel : public UStaticMeshComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHoistControlPanel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	class UHoistComponent* GetHoistComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool PowerIsOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ExtendCommand = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* JettisonMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* PowerSwitchMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* UpDownMeshComponent = nullptr;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetPowerState(bool power);

	UFUNCTION(BlueprintCallable)
	bool GetPowerState();

	UFUNCTION(BlueprintCallable)
	void SetExtendCommand(float extend);

	UFUNCTION(BlueprintCallable)
	void Jettison();
};
