// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/ExponentialHeightFogComponent.h"

#include "AccurateDayNightRules.h"
#include "MoonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACCURATEDAYNIGHT_API UMoonComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMoonComponent();

public:
	/**
		Components
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "Default", OverrideNativeName = "NightSkyMaterial"))
		UMaterialInstanceDynamic* NightSkyMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "Default", OverrideNativeName = "MoonLight"))
		UDirectionalLightComponent* MoonLight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true", Category = "Default", OverrideNativeName = "SkySphereMesh"))
		UStaticMeshComponent* SkySphereMesh;

public:

	void SetMoonRotation(const FRotator& MoonRotator);
	void MoveMoon();

	void SetMoonEnable(float MoonEnable);
	void SetMoonRadius(float MoonRadius);
	void SetMoonIntensity(float MoonLightStrength);
	void SetStarsBrightness(float StarsBrightness);

	void SetMoonRules(FMoonRules MoonRulesInput);

private:
	UMaterialInterface* NightSkyMaterialFound;
	FMoonRules MoonRules;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
