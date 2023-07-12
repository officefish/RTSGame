// Fill out your copyright notice in the Description page of Project Settings.


#include "SunComponent.h"

// Sets default values for this component's properties
USunComponent::USunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SunDirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalLight"));

	SunDirectionalLight->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	SunDirectionalLight->SetMobility(EComponentMobility::Movable);

	SunDirectionalLight->Intensity = 10;
	SunDirectionalLight->ULightComponent::SetIntensity(10.000000);
	//Comment this one in UE5 Preview2
	SunDirectionalLight->bUsedAsAtmosphereSunLight = true;
	//Uncomment this one in UE5 Preview2
	//DirectionalLight->bAtmosphereSunLight = true;
	SunDirectionalLight->LightSourceAngle = 0.500000f;
	SunDirectionalLight->DynamicShadowCascades = 5;
	SunDirectionalLight->CascadeDistributionExponent = 1.400000f;
	// ...

	MaxIntensity = 10.0f;
}

void USunComponent::SetSunRotation(const FRotator& SunRotator)
{
	/*if (::IsValid(SunDirectionalLight) &&
		SunDirectionalLight->Mobility.GetValue() == EComponentMobility::Movable
		)
	{*/
		UE_LOG(LogTemp, Warning, TEXT("SetSunRotation"));
		this->SetWorldRotation(SunRotator);
	//}
}

void USunComponent::SetIntensity(float Value)
{
	if (::IsValid(SunDirectionalLight))
	{
		SunDirectionalLight->SetIntensity(Value * MaxIntensity);
	}
}

// Called when the game starts
void USunComponent::BeginPlay() 
{
	Super::BeginPlay();
}



