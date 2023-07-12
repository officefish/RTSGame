// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
#include "AccurateDayNightStateBase.generated.h"

UENUM(BlueprintType)
enum class EEGameSpeed : uint8
{
	Pause			UMETA(DisplayName = "Pause"),
	SlowMotion      UMETA(DisplayName = "SlowMotion"),
	RealTime		UMETA(DisplayName = "RealTime"),
	GameTime		UMETA(DisplayName = "GameTime"),
	GameTimeX2		UMETA(DisplayName = "GameTimeX2"),
	GameTimeX5		UMETA(DisplayName = "GameTimeX5"),
	GameTimeX50		UMETA(DisplayName = "GameTimeX50"),
	GameTimeX500	UMETA(DisplayName = "GameTimeX500"),
};

UENUM(BlueprintType)
enum class EEDayNightCicle : uint8
{
	BeforeSunsetStart		UMETA(DisplayName = "BeforeSunsetStart"),
	SunsetStart				UMETA(DisplayName = "SunsetStart"),
	NightStart				UMETA(DisplayName = "NightStart"),
	BeforeSunriseStart		UMETA(DisplayName = "BeforeSunriseStart"),
	SunriseStart			UMETA(DisplayName = "SuneriseStart"),
	DayStart				UMETA(DisplayName = "DayStart"),
};

UENUM(BlueprintType)
enum class EEDayNightSchedule : uint8
{
	Night			UMETA(DisplayName = "Night"),
	LateNight		UMETA(DisplayName = "LateNight"),
	Morning			UMETA(DisplayName = "Morning"),
	LateMorning		UMETA(DisplayName = "LateMorning"),
	Day				UMETA(DisplayName = "Day"),
	LateDay			UMETA(DisplayName = "LateDay"),
	Evening			UMETA(DisplayName = "Evening"),
	LateEvening		UMETA(DisplayName = "LateEvening"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDayNightCicleDelegate, TEnumAsByte<EEDayNightCicle>, EDayNightCycle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDayNightScheduleDelegate, TEnumAsByte<EEDayNightSchedule>, EDayNightSchedule);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameSpeedDelegate, TEnumAsByte<EEGameSpeed>, EGameSpeed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStateTickDelegate, FDateTime, TickDateTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewDayDelegate);

/**
 * 
 */
UCLASS()
class ACCURATEDAYNIGHT_API AAccurateDayNightStateBase : public AGameStateBase
{
	GENERATED_BODY()

	AAccurateDayNightStateBase();

public:

	UFUNCTION(BlueprintCallable, Category = "GameDatetime")
		void GameSpeedUp();

	UFUNCTION(BlueprintCallable, Category = "GameDatetime")
		void GameSpeedDown();

	UFUNCTION(BlueprintPure, Category = "GameDatetime", meta = (DefaultToSelf = Target))
		float GetGameCurrentDaySeconds();

	UFUNCTION(BlueprintPure, Category = "GameDatetime", meta = (DefaultToSelf = Target))
		float GetGameSolarTime();

	UFUNCTION(BlueprintCallable, Category = "AccurateDayNight")
		void SetGameSpeed(TEnumAsByte<EEGameSpeed> EGameSpeedStatus);

	UFUNCTION(BlueprintCallable, Category = "AccurateDayNight")
		void SetGameTimeMultiplier(float Value);

	UFUNCTION(BlueprintPure, Category = "AccurateDayNight")
		float GetGameTimeMultiplier();

	UFUNCTION(BlueprintCallable, Category = "AccurateDayNight")
		void SetGameStartDateTime(const FDateTime& DateTime);


	// Day/Night Transition Setup
	UFUNCTION(BlueprintCallable, Category = "GameDatetime")
		void SetDayNightTransitionTime(int32 Hour, int32 Minute);

	UFUNCTION(BlueprintPure, Category = "GameDatetime")
		FTimespan GetDayNightTransitionTime();

protected:

	virtual void Tick(float DeltaTime) override;

public:

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "AccurateDayNight")
		FDayNightCicleDelegate OnDayNightCycleChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "AccurateDayNight")
		FDayNightScheduleDelegate OnDayNightScheduleChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "AccurateDayNight")
		FGameSpeedDelegate OnGameSpeedChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "AccurateDayNight")
		FNewDayDelegate OnNewDayDelegate;

	UPROPERTY(BlueprintAssignable, Category = "AccurateDayNight")
		FGameStateTickDelegate OnGameStateTickDelegate;

	// DayNightCicle
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		FTimespan SunsetTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		FTimespan SunriseTime;

	// Enum example
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AccurateDayNight")
		TEnumAsByte<EEGameSpeed> EGameSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AccurateDayNight")
		TEnumAsByte<EEDayNightCicle> EDayNightCycle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AccurateDayNight")
		TEnumAsByte<EEDayNightSchedule> EDayNightSchedule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AccurateDayNight")
		FDateTime FullDateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		bool bWithDayNightCycle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccurateDayNight")
		FDateTime GameStartDatetime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		bool bWithSlowMotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		bool bWithRealTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		bool bWithX50Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		bool bWithX500Time;

	/* Day/Night Transition Time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDayNight")
		FTimespan DayNightTransitionTime;

private:

	float GetTimeMuliplier();
	TEnumAsByte<EEDayNightCicle> DefineDayNightCycle(const int32& NumMinutes);
	TEnumAsByte<EEDayNightSchedule> DefineDayNightSchedule(const int32& NumHours);

protected:

	const float NUM_MINUTES = 59.99f;
	const float NUM_SECONDS = 3599.99f;
	const float NUM_MILISECONDS = 3599999.99f;

private:

	float GameTimeMultiplier;
	float GameTimeMultiplierX2;
	float GameTimeMultiplierX5;
	float GameTimeMultiplierX50;
	float GameTimeMultiplierX500;

	/* Total time passed after BeginPlay */

	FDateTime GameStartDateTime;
	FTimespan TimePasses;

	float TotalSeconds;	
};
