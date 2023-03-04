// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("Trigger component created"));
}

void UTriggerComponent::SetMover(UMover* mover)
{
	m_MoverComponent = mover;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Trigger component alive"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* foundActor = GetAcceptableActor();
	if (foundActor != nullptr)
	{
		auto rootComponent = Cast<UPrimitiveComponent>(foundActor->GetRootComponent());
		if (rootComponent != nullptr) // If it is a primitive component
		{
			rootComponent->SetSimulatePhysics(false);
		}

		foundActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

		m_MoverComponent->SetShouldMove(true);
	}
	else
	{
		m_MoverComponent->SetShouldMove(false);
	}
}

AActor* UTriggerComponent::GetAcceptableActor()
{
	TArray<AActor*> actors{};
	GetOverlappingActors(actors);

	for (AActor* actor : actors)
	{
		bool HasAcceptableTag = actor->ActorHasTag(ListenToOverlapWithTag);
		bool IsGrabbed = actor->ActorHasTag(DontTriggerOnTag);

		if (HasAcceptableTag && !IsGrabbed)
		{
			return actor;
		}
	}

	return nullptr;
}
