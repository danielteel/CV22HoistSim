#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Grabbable.generated.h"


UINTERFACE(MinimalAPI)
class UGrabbable : public UInterface {
	GENERATED_BODY()
};


class CV22HOISTSIM_API IGrabbable {
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	USceneComponent* GetComponentToGrab();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	void GrabStart(class UPrimitiveComponent* hand);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	void GrabEnd(class UPrimitiveComponent* hand);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	void GrabEvent(class UPrimitiveComponent* hand, bool buttonPressed, float xAxis, float yAxis);
};
