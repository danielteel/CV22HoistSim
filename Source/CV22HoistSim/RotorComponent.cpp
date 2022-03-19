#include "RotorComponent.h"

#include "HoistComponent.h"
#include "Engine/World.h"
#include "HoistComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Physics/PhysScene_PhysX.h"
#include "Physics/PhysicsInterfaceCore.h"
#include "PhysXPublic.h"
#include "Engine/StaticMesh.h"
#include "PhysicsEngine/BodyInstance.h"
#include "GameFramework/PhysicsVolume.h"
#include "CV22HoistSimGameModeBase.h"
#include "Kismet/GameplayStatics.h"

URotorComponent::URotorComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void URotorComponent::BeginPlay() {
	Super::BeginPlay();
}

void URotorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AddLocalRotation(FRotator(0.0f, 1900.0f * DeltaTime * (IsLeftRotor ? 1.0f : -1.0f), 0.0f));
	ApplyWindForces();
}

UHoistComponent* URotorComponent::GetHoistComponent() {
	AActor* owner = GetOwner();
	if (owner) {
		return owner->FindComponentByClass<UHoistComponent>();
	}
	return nullptr;
}


FVector URotorComponent::GetWindForce(UPrimitiveComponent* component, FVector windDirection, float windSpeed) {
	float componentSurfaceArea = GetWindSurfaceArea(component);
	float force = windSpeed / 200.0f * componentSurfaceArea;
	return windDirection * force;
}

void URotorComponent::ApplyWindForces() {
	UHoistComponent* hoist = GetHoistComponent();
	if (!hoist) return;
	ACV22HoistSimGameModeBase* gameMode = Cast<ACV22HoistSimGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) return;


	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	TArray<UPrimitiveComponent*> windAffectableComponents = hoist->GetWindAffectableComponents();
	params.AddIgnoredComponents(windAffectableComponents);
	if (windAffectableComponents.Num() <= 0) return;

	TArray<FVector> componentForce;
	TArray<float> componentDistFromRayCenter;
	for (int i = 0; i < windAffectableComponents.Num(); i++) {
		componentForce.Add(FVector(0.0f));
		componentDistFromRayCenter.Add(10000000.0f);
	}

	FVector propCenter = GetComponentLocation();
	FVector forwardVector = GetOwner()->GetActorForwardVector();
	FVector rightVector = GetOwner()->GetActorRightVector();
	FVector downVector = FVector(0.0f) - GetUpVector();
	FVector windOffset = FVector(WindSpeed * 50, 0.0f, 0.0f).RotateAngleAxis(WindDirection + 180.0f, FVector(0.0f, 0.0f, 1.0f));
	FHitResult hitResult;


	FMath::SRandInit((int32)(GetWorld()->TimeSeconds));

	for (int angle = 160; angle < 380; angle += 10) {

		DownwashCurrentGust = FMath::SRand() * DownwashMaxGust;

		FVector rightward = rightVector * (FMath::Cos(FMath::DegreesToRadians(float(angle)))*DiscRadius / 2);
		FVector forward = forwardVector * (FMath::Sin(FMath::DegreesToRadians(float(angle)))*DiscRadius / 2);

		FVector start = propCenter + rightward + forward;
		FVector end = start + (downVector * DownwashZeroHeight) + windOffset;
		FVector pushOutNormal = (start - propCenter).GetSafeNormal();

		//DrawDebugDirectionalArrow(GetWorld(), start, end, 150, FColor(0, 0, 255), false, -1.0f, 0, 80.0f);
		bool result = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);
		if (result) {
			FVector groundFollowNormal = FVector::VectorPlaneProject((end - start), hitResult.Normal).GetSafeNormal();
			FVector downwashNormal = ((pushOutNormal*1.75f + groundFollowNormal) / 2.75f).GetSafeNormal();
			float downwashSpeed = DownwashSpeedAtRotors + DownwashCurrentGust - (hitResult.Distance*DownwashSpeedReduceFactor);

			//DrawDebugCylinder(GetWorld(), hitResult.Location, hitResult.Location + downwashNormal * downwashSpeed, DownwashRadius/4,16, FColor(0, 0, 255), false, -1.0f, 0, 5.0f);
			//DrawDebugDirectionalArrow(GetWorld(), hitResult.Location, hitResult.Location + downwashNormal * downwashSpeed,150, FColor(0, 0, 255), false, -1.0f, 0, 80.0f);

			FRay downwashRay = FRay(hitResult.Location, downwashNormal, true);

			for (int i = 0; i < windAffectableComponents.Num(); i++) {
				auto component = windAffectableComponents[i];
				FVector componentLocation = component->GetComponentLocation();
				FVector closestPoint = downwashRay.ClosestPoint(componentLocation);

				float hookDistFromRay = FVector::Dist(closestPoint, componentLocation);
				if (hookDistFromRay < DownwashRadius) {
					if (hookDistFromRay < componentDistFromRayCenter[i]) {
						componentDistFromRayCenter[i] = hookDistFromRay;

						float effectiveWindSpeed = downwashSpeed - (DownwashSpeedReduceFactor * 1.5f * (FVector::Dist(componentLocation, hitResult.Location)));
						if (effectiveWindSpeed < 0.0f) effectiveWindSpeed = 0.0f;
						componentForce[i] = GetWindForce(component, downwashNormal, effectiveWindSpeed);
					}
				}
			}
		}
	}
	for (int i = 0; i < windAffectableComponents.Num(); i++) {
		if (!componentForce[i].IsNearlyZero()) {
			FVector componentForceSigns = componentForce[i].GetSignVector();
			FVector currentForce = windAffectableComponents[i]->GetPhysicsLinearVelocity()*windAffectableComponents[i]->GetMass();
			FVector currentForceSigns = currentForce.GetSignVector();
			if (!FMath::IsNearlyEqual(componentForceSigns.X, currentForceSigns.X)) currentForce.X = 0.0f;
			if (!FMath::IsNearlyEqual(componentForceSigns.Y, currentForceSigns.Y)) currentForce.Y = 0.0f;
			if (!FMath::IsNearlyEqual(componentForceSigns.Z, currentForceSigns.Z)) currentForce.Z = 0.0f;
			componentForce[i] = (componentForce[i] - currentForce) / 1.5f;
			windAffectableComponents[i]->AddForce(componentForce[i] * gameMode->KickFactor);
		}
	}

}

float URotorComponent::GetWindSurfaceArea(UPrimitiveComponent* primitive) {
	float volume = 0.0f;
	PxScene* scene = GetWorld()->GetPhysicsScene()->GetPxScene();
	if (scene) {
		TArray<FPhysicsShapeHandle> collisionShapes;
		scene->lockRead();
		int32 numSyncShapes = primitive->BodyInstance.GetAllShapes_AssumesLocked(collisionShapes);
		scene->unlockRead();

		PxBoxGeometry boxGeo;
		PxSphereGeometry sphereGeo;
		PxCapsuleGeometry capsuleGeo;
		float w, h, l;
		for (auto& shape : collisionShapes) {
			switch (shape.Shape->getGeometryType()) {
			case PxGeometryType::eBOX:
				shape.Shape->getBoxGeometry(boxGeo);
				if (boxGeo.isValid()) {
					w = boxGeo.halfExtents.x * 2.0f;
					l = boxGeo.halfExtents.y * 2.0f;
					h = boxGeo.halfExtents.z * 2.0f;
					volume += w * h*l;
				}
				break;
			case PxGeometryType::eCAPSULE:
				shape.Shape->getCapsuleGeometry(capsuleGeo);
				if (capsuleGeo.isValid()) {
					//Add spheres volume
					volume += 4.188f*FMath::Pow(capsuleGeo.radius, 3.0f);
					//Add cylinders volume
					h = capsuleGeo.halfHeight * 2.0f;
					volume += 3.141f*FMath::Pow(capsuleGeo.radius, 2.0f)*h;
				}
				break;
			case PxGeometryType::eSPHERE:
				shape.Shape->getSphereGeometry(sphereGeo);
				if (sphereGeo.isValid()) {
					volume += 4.188f*FMath::Pow(sphereGeo.radius, 3.0f);
				}
				break;
			}
		}
	}
	float radiusOfOverallSphere = FMath::Pow(volume / 4.188f, 0.333f);
	float circleSurfaceArea = 3.141f*FMath::Pow(radiusOfOverallSphere, 2.0f);
	return circleSurfaceArea;
}
