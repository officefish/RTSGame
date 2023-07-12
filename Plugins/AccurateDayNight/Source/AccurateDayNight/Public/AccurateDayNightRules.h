#pragma once

#include "CoreMinimal.h"
#include "SunPosition.h"
#include "AccurateDayNightRules.generated.h"

USTRUCT(BlueprintType)
struct FDayLightSavingsTime
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "DateTimeTick"))
		bool bEnable; //UseDaylightSavingsTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "DateTimeTick"))
		int32 StartMonth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "DateTimeTick"))
		int32 StartDay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "DateTimeTick"))
		int32 EndMonth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "DateTimeTick"))
		int32 EndDay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "DateTimeTick"))
		int32 SwitchHours;
};

USTRUCT(BlueprintType)
struct FMoonRules
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		bool Moon;

	UPROPERTY(BlueprintReadWrite)
		float MoonLightStrength;

	UPROPERTY(BlueprintReadWrite)
		float MoonDistance;

	UPROPERTY(BlueprintReadWrite)
		float MoonSize;

	UPROPERTY(BlueprintReadWrite)
		float MoonEnable;

	UPROPERTY(BlueprintReadWrite)
		float MaxStarsBrightness;

};


USTRUCT(BlueprintType)
struct FDayNightCycleRules
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		int myNumber;

	UPROPERTY(BlueprintReadWrite)
		FDateTime RealWorld_StaticStartTime;

	UPROPERTY(BlueprintReadWrite)
		FDateTime GameWorld_StaticStartTime;

	UPROPERTY(BlueprintReadWrite)
		FTimespan RealWorldTimeInGameTime;

	UPROPERTY(BlueprintReadWrite)
		float Latitude;

	UPROPERTY(BlueprintReadWrite)
		float Longitude;

	UPROPERTY(BlueprintReadWrite)
		float TimeZone;

	UPROPERTY(BlueprintReadWrite)
		float NorthOffset;
};

USTRUCT(BlueprintType)
struct FSunPositionRules
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "Location"))
		float Latitude;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "Location"))
		float Longitude;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "Location"))
		float TimeZone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "Location"))
		float NorthOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "Location"))
		FSunPositionData Data;
};