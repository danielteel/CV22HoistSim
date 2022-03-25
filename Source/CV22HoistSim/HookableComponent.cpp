#include "HookableComponent.h"
#include "Components/PrimitiveComponent.h"
#include "HookInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"

UHookableComponent::UHookableComponent(){
	PrimaryComponentTick.bCanEverTick = true;
}


void UHookableComponent::OnRegister() {
	Super::OnRegister();
	PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this, FName("PhysicsConstraint"));
	PhysicsConstraint->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	PhysicsConstraint->RegisterComponent();
}

void UHookableComponent::BeginPlay(){
	Super::BeginPlay();
	PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 60.0f);
	PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 60.0f);
	PhysicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = 1;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Damping = 800.0f;
	PhysicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Stiffness = 10.0f;
}

void UHookableComponent::OnComponentDestroyed(bool bDestroyingHierarchy) {
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}


void UHookableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ComponentHookedTo && FVector::Distance(ComponentHookedTo->GetSocketLocation(FName("Hook")), GetSocketLocation(FName("Hook")))>10.0f) {//Keep trying to make that constraint work
		FVector delta = ComponentHookedTo->GetSocketLocation(FName("Hook")) - GetSocketLocation(FName("Hook"));
		AddWorldOffset(delta);
		PhysicsConstraint->SetWorldLocation(GetSocketLocation(FName("Hook")));
		PhysicsConstraint->SetConstrainedComponents(this, NAME_None, ComponentHookedTo, NAME_None);
		UE_LOG(LogTemp, Warning, TEXT("Well well well"))
	}
}


UPrimitiveComponent* UHookableComponent::GetHookComponent() {
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> actorsToIgnore;
	TArray<UPrimitiveComponent*> outComponents;

	UPrimitiveComponent* componentToHookTo = nullptr;

	FVector ourLocation = GetSocketLocation(FName("Hook"));

	bool result = UKismetSystemLibrary::SphereOverlapComponents(GetWorld(), ourLocation, HookDistance, objectTypes, nullptr, actorsToIgnore, outComponents);
	if (result) {
		float distanceToHook = 0.0f;
		for (int i = 0; i < outComponents.Num(); i++) {
			bool isHookObject = outComponents[i]->Implements<UHookInterface>();
			if (isHookObject) {
				float distToObj = FVector::Dist(ourLocation, outComponents[i]->GetSocketLocation(FName("Hook")));
				if (distToObj < distanceToHook || componentToHookTo == nullptr) {
					componentToHookTo = outComponents[i];
					distanceToHook = distToObj;
				}
			}
		}
	}
	return componentToHookTo;
}

USceneComponent* UHookableComponent::GetComponentToGrab_Implementation() {
	return this;
}

void UHookableComponent::GrabStart_Implementation(UPrimitiveComponent * hand) {
	PhysicsConstraint->BreakConstraint();	
	SetSimulatePhysics(false);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ComponentHookedTo = nullptr;
	AttachToComponent(hand, FAttachmentTransformRules::KeepWorldTransform);
}

void UHookableComponent::GrabEnd_Implementation(UPrimitiveComponent * hand) {
	DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetSimulatePhysics(true);
	ComponentHookedTo = GetHookComponent();
	if (ComponentHookedTo) {
		FVector delta = ComponentHookedTo->GetSocketLocation(FName("Hook"))-GetSocketLocation(FName("Hook"));
		AddWorldOffset(delta);
		PhysicsConstraint->SetWorldLocation(GetSocketLocation(FName("Hook")));
		PhysicsConstraint->SetConstrainedComponents(this, NAME_None, ComponentHookedTo, NAME_None);
	}
}