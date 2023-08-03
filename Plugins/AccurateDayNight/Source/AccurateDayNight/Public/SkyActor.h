// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SceneComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"

#include "Components/StaticMeshComponent.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/TimelineComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"

#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

#include "SunComponent.h"
#include "MoonComponent.h"
#include "CompassComponent.h"
#include "AccurateDayNightRules.h"

#include "CultureGameStateBase.h"

#include "SunPosition.h"
#include "SkyActor.generated.h"

UCLASS()
class ACCURATEDAYNIGHT_API ASkyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyActor();

public:

	/**
	 Functions
	*/

	// GameState Listeners
	UFUNCTION()
		void OnDayNightCycleChange(TEnumAsByte<EEDayNightCicle> EDayNightCicle);

	UFUNCTION()
		void OnGameStateTick(FDateTime CurrentDateTime);

	UFUNCTION()
		void OnNextDay();

	UFUNCTION(BlueprintPure)
		FTimespan GetSunsetTime();

	UFUNCTION(BlueprintPure)
		FTimespan GetSunriseTime();

	UFUNCTION(BlueprintCallable)
		void GetCurrentRotator(FRotator& CurrentRotator, FSunPositionRules& Rules);

	UFUNCTION(BlueprintCallable)
		void UpdateSun(const FRotator& SunRotator);

	UFUNCTION(BlueprintCallable)
		void UpdateMoon(const FRotator& MoonRotator);

	UFUNCTION(BlueprintCallable)
		void UpdateCompass(const FQuat& CompassQuaternion);

	UFUNCTION(BlueprintCallable)
		void UpdateSky();

	UFUNCTION(BlueprintCallable)
		void SetMoonRules(FMoonRules MoonRulesInput);

	UFUNCTION(BlueprintPure)
		bool IsDST(FDayLightSavingsTime DSTStruct);

	//DayNight Cycle Change

	UFUNCTION(BlueprintCallable)
		void OnBeforeSunset(float Modifier, float InverseModifier);

	UFUNCTION(BlueprintCallable)
		void OnSunset(float Modifier, float InverseModifier);

	UFUNCTION(BlueprintCallable)
		void OnNight(float Modifier, float InverseModifier);

	UFUNCTION(BlueprintCallable)
		void OnBeforeSunrise(float Modifier, float InverseModifier);

	UFUNCTION(BlueprintCallable)
		void OnSunrise(float Modifier, float InverseModifier);

	UFUNCTION(BlueprintCallable)
		void OnDay(float Modifier, float InverseModifier);

public:
	virtual void BeginPlay() override;

public:

	/**
	 Components
	*/

	UPROPERTY(BlueprintReadWrite, NonTransactional, meta = (Category = "AccurateDayNight", OverrideNativeName = "Scene"))
		USceneComponent* Scene;

	// Sky
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "AccurateDayNight", OverrideNativeName = "SkyLight"))
		USkyLightComponent* SkyLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "AccurateDayNight", OverrideNativeName = "SkyAtmosphere"))
		USkyAtmosphereComponent* SkyAtmosphere;

	// Sun
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "AccurateDayNight", OverrideNativeName = "SunComponent"))
		USunComponent* SunComponent;

	//Compass 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "AccurateDayNight", OverrideNativeName = "CompassComponent"))
		UCompassComponent* CompassComponent;

	//Moon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "AccurateDayNight", OverrideNativeName = "MoonComponent"))
		UMoonComponent* MoonComponent;
	
	// Exponential Height Fog
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category = "AccurateDayNight", OverrideNativeName = "ExponentialHeightFog"))
		UExponentialHeightFogComponent* ExponentialHeightFog;



public:

	/**
	 Variables
	*/

	//Location
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "Location"))
		FSunPositionRules SunPositionRules;

	//Date
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "DateTimeTick"))
		FDayLightSavingsTime DayLightSavingsTime;

	UPROPERTY(BlueprintReadWrite)
		FDayNightCycleRules DayNightRules;

	UPROPERTY(BlueprintReadWrite)
		FMoonRules MoonRules;


private:

	// GameState pointer 
	ACultureGameStateBase* ASkyActor::GetGameState() const;
	void UpdateGameStateData();

	void SetupDefaults();
	void ConstructSubObjects();
	void SetupSubObjects();

	void SmoothUpdate();
	float GetSmoothModifier() const;

	void SetFogAlpha(float FogAlpha);

	void ShowFog();
	void HideFog();

	inline bool SureSunComponent() 
	{
		return ensure(SunComponent != nullptr);
	}

	inline bool SureCompassComponent()
	{
		return ensure(CompassComponent != nullptr);
	}

	inline bool SureMoonComponent()
	{
		return ensure(MoonComponent != nullptr);
	}

	inline bool SureSkyLight()
	{
		return ensure(SkyLight != nullptr);
	}

	inline bool SureFog()
	{
		return ensure(ExponentialHeightFog != nullptr);
	}


protected:
	const float ALBEDO_COLOR = 0.603827;
	const float MAX_VALUE = 1.0f;
	const float MIN_VALUE = 0.0f;

private:

	bool bFirstTick;
	bool bWithDay;
	bool bWithNight;

	bool bOnNightBegin;
	bool bOnDayBegin;

	ACultureGameStateBase* GameState;
	TEnumAsByte<EEDayNightCicle> ECurrentDayNightCicle;

	FDateTime TickDateTime;
	FDateTime AnimationBeginDateTime;
	float AnimationTotalSeconds;

	float FogDensity;
	float FogMaxOpacity;

};
