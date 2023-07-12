// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/DirectionalLightComponent.h"

#include "SunComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACCURATEDAYNIGHT_API USunComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USunComponent();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "Default", OverrideNativeName = "DirectionalLight"))
		UDirectionalLightComponent* SunDirectionalLight;

public:
	void SetSunRotation(const FRotator& SunRotator);
	void SetIntensity(float Value);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float MaxIntensity;
};
