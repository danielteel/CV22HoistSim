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
	bool GrabStart(class UPrimitiveComponent* hand);//Return false if object doesnt want grabbed

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	bool GrabEnd(class UPrimitiveComponent* hand);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Grabbable")
	bool GrabEvent(class UPrimitiveComponent* hand, bool buttonPressed, float xAxis, float yAxis);
};
