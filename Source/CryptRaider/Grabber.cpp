// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsComponent = GetPhysicsHandle();
	if (PhysicsComponent && PhysicsComponent->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsComponent->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent"));
	}

	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& hitOut) const
{
	const FVector Start = GetComponentLocation();
	const FVector End = Start + GetForwardVector() * MaxGrabDistance;
	const UWorld* World = GetWorld();
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return World->SweepSingleByChannel(
		hitOut,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		Sphere
	);
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsComponent = GetPhysicsHandle();
	if (PhysicsComponent == nullptr)
	{
		return;
	}

	if (FHitResult HitResult{}; GetGrabbableInReach(HitResult))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();

		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add(TagToAddOnGrab);
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		PhysicsComponent->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsComponent = GetPhysicsHandle();

	if (PhysicsComponent && PhysicsComponent->GetGrabbedComponent())
	{
		auto grabbedActor = PhysicsComponent->GetGrabbedComponent()->GetOwner();
		grabbedActor->Tags.Remove(TagToAddOnGrab);
		PhysicsComponent->ReleaseComponent();
	}
}

