#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"

#include "TriggerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* mover);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* GetAcceptableActor();

private:
	UMover* m_MoverComponent{};

	UPROPERTY(EditAnywhere)
	FName ListenToOverlapWithTag {};

	
	UPROPERTY(EditAnywhere)
	FName DontTriggerOnTag {"Grab" };
};
