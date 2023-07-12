// Fill out your copyright notice in the Description page of Project Settings.


#include "AccurateDayNightStateBase.h"


AAccurateDayNightStateBase::AAccurateDayNightStateBase()
	: AGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bWithSlowMotion = false;
	bWithX50Time = false;
	bWithRealTime = true;
	EGameSpeed = EEGameSpeed::GameTime;

	SetGameTimeMultiplier(60.0f);
	SetGameStartDateTime(FDateTime::UtcNow());

	SunriseTime = FTimespan::FromHours(8.0f);
	SunsetTime = FTimespan::FromHours(21.0f);

	DayNightTransitionTime = FTimespan::FromMinutes(35.0f);

	bWithDayNightCycle = true;



}

void AAccurateDayNightStateBase::SetGameSpeed(TEnumAsByte<EEGameSpeed> EGameSpeedStatus)
{
	EGameSpeed = EGameSpeedStatus;
	OnGameSpeedChangeDelegate.Broadcast(EGameSpeed);
}

void AAccurateDayNightStateBase::SetGameTimeMultiplier(float Value)
{
	GameTimeMultiplier = Value;
	GameTimeMultiplierX2 = Value * 2;
	GameTimeMultiplierX5 = Value * 5;
	GameTimeMultiplierX50 = Value * 50;
	GameTimeMultiplierX500 = Value * 500;
}

float AAccurateDayNightStateBase::GetGameTimeMultiplier()
{
	return GameTimeMultiplier;
}

void AAccurateDayNightStateBase::SetGameStartDateTime(const FDateTime& DateTime)
{
	GameStartDateTime = DateTime;

	//FString FullDateStr = GameStartDateTime.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("GameStartDateTime: %s"), *FullDateStr);

	int NumHours = GameStartDatetime.GetHour();
	EDayNightCycle = DefineDayNightCycle(NumHours);
	TimePasses = FTimespan(0, 0, 0, 0);
	FullDateTime = DateTime;
	TotalSeconds = 0.0f;
}



float AAccurateDayNightStateBase::GetTimeMuliplier()
{
	if (EGameSpeed == EEGameSpeed::Pause) {
		return 0.0f;
	}
	else if (EGameSpeed == EEGameSpeed::SlowMotion) {
		return 0.5f;
	}
	else if (EGameSpeed == EEGameSpeed::RealTime) {
		return 1.0f;
	}
	else if (EGameSpeed == EEGameSpeed::GameTime) {
		return GameTimeMultiplier;
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX2) {
		return GameTimeMultiplierX2;
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX5) {
		return GameTimeMultiplierX5;
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX50) {
		return GameTimeMultiplierX50;
	}
	else {
		return GameTimeMultiplierX500;
	}
}


void AAccurateDayNightStateBase::GameSpeedUp()
{
	bool bNeedBroadcast = true;

	if (EGameSpeed == EEGameSpeed::Pause) {
		if (bWithSlowMotion) {
			EGameSpeed = EEGameSpeed::SlowMotion;
		}
		else if (bWithRealTime) {
			EGameSpeed = EEGameSpeed::RealTime;
		}
		else {
			EGameSpeed = EEGameSpeed::GameTime;
		}
	}
	else if (EGameSpeed == EEGameSpeed::SlowMotion) {
		if (bWithRealTime) {
			EGameSpeed = EEGameSpeed::RealTime;
		}
		else {
			EGameSpeed = EEGameSpeed::GameTime;
		}
	}
	else if (EGameSpeed == EEGameSpeed::RealTime) {
		EGameSpeed = EEGameSpeed::GameTime;
	}
	else if (EGameSpeed == EEGameSpeed::GameTime) {
		EGameSpeed = EEGameSpeed::GameTimeX2;
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX2) {
		EGameSpeed = EEGameSpeed::GameTimeX5;
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX5) {
		if (bWithX50Time) {
			EGameSpeed = EEGameSpeed::GameTimeX50;
		}
		else {
			if (bWithX500Time) {
				EGameSpeed = EEGameSpeed::GameTimeX500;
			}
			else {
				bNeedBroadcast = false;
			}
		}
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX50) {
		if (bWithX500Time) {
			EGameSpeed = EEGameSpeed::GameTimeX500;
		}
		else {
			bNeedBroadcast = false;
		}
	}
	else {
		bNeedBroadcast = false;
	}

	if (bNeedBroadcast) {
		OnGameSpeedChangeDelegate.Broadcast(EGameSpeed);
	}
}

void AAccurateDayNightStateBase::GameSpeedDown()
{
	bool bNeedBroadcast = true;
	if (EGameSpeed == EEGameSpeed::GameTimeX500) {
		if (bWithX50Time) {
			EGameSpeed = EEGameSpeed::GameTimeX50;
		}
		else {
			EGameSpeed = EEGameSpeed::GameTimeX5;
		}
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX50) {
		EGameSpeed = EEGameSpeed::GameTimeX5;
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX5) {
		EGameSpeed = EEGameSpeed::GameTimeX2;
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX2) {
		EGameSpeed = EEGameSpeed::GameTime;
	}
	else if (EGameSpeed == EEGameSpeed::GameTime) {
		if (bWithRealTime) {
			EGameSpeed = EEGameSpeed::RealTime;
		}
		else if (bWithSlowMotion) {
			EGameSpeed = EEGameSpeed::SlowMotion;
		}
		else {
			EGameSpeed = EEGameSpeed::Pause;
		}
	}
	else if (EGameSpeed == EEGameSpeed::RealTime) {
		if (bWithSlowMotion) {
			EGameSpeed = EEGameSpeed::SlowMotion;
		}
		else {
			EGameSpeed = EEGameSpeed::Pause;
		}
	}
	else if (EGameSpeed == EEGameSpeed::SlowMotion) {
		EGameSpeed = EEGameSpeed::Pause;
	}
	else {
		bNeedBroadcast = false;
	}

	if (bNeedBroadcast) {
		OnGameSpeedChangeDelegate.Broadcast(EGameSpeed);
	}
}

TEnumAsByte<EEDayNightCicle> AAccurateDayNightStateBase::DefineDayNightCycle(const int32& NumMinutes)
{
	int32 TransitionMinures = DayNightTransitionTime.GetHours() * 60 + DayNightTransitionTime.GetMinutes();
	int32 SunsetMinutes = SunsetTime.GetHours() * 60 + SunsetTime.GetMinutes();
	int32 SunriseMinutes = SunriseTime.GetHours() * 60 + SunriseTime.GetMinutes();

	int32 BeforeSunsetMinutes = SunsetMinutes - TransitionMinures;
	int32 NightMinutes = SunsetMinutes + TransitionMinures;

	int32 BeforeSunriseMinutes = SunriseMinutes - TransitionMinures;
	int32 DayMinutes = SunriseMinutes + TransitionMinures;

	/*UE_LOG(LogTemp, Warning, TEXT("Before Sunrise: %d"), BeforeSunriseMinutes);
	UE_LOG(LogTemp, Warning, TEXT("Sunrise: %d"), SunriseMinutes);
	UE_LOG(LogTemp, Warning, TEXT("Day: %d"), DayMinutes);
	UE_LOG(LogTemp, Warning, TEXT("Before Sunset: %d"), BeforeSunsetMinutes);
	UE_LOG(LogTemp, Warning, TEXT("Sunset: %d"), SunsetMinutes);
	UE_LOG(LogTemp, Warning, TEXT("Night: %d"), NightMinutes);*/

	if (NumMinutes < BeforeSunriseMinutes) {
		return EEDayNightCicle::NightStart;
	}
	else if (NumMinutes >= BeforeSunriseMinutes &&
		NumMinutes < SunriseMinutes) {
		return EEDayNightCicle::BeforeSunriseStart;
	}
	else if (NumMinutes >= SunriseMinutes &&
		NumMinutes < DayMinutes) {
		return EEDayNightCicle::SunriseStart;
	}
	else if (NumMinutes >= DayMinutes &&
		NumMinutes < BeforeSunsetMinutes) {
		return EEDayNightCicle::DayStart;
	}
	else if (NumMinutes >= BeforeSunsetMinutes &&
		NumMinutes < SunsetMinutes) {
		return EEDayNightCicle::BeforeSunsetStart;
	}
	else if (NumMinutes >= SunsetMinutes &&
		NumMinutes < NightMinutes) {
		return EEDayNightCicle::SunsetStart;
	}
	else {
		return EEDayNightCicle::NightStart;
	}
}

TEnumAsByte<EEDayNightSchedule> AAccurateDayNightStateBase::DefineDayNightSchedule(const int32& NumHours)
{
	if (NumHours < 3) {
		return EEDayNightSchedule::Night;
	}
	if (NumHours >= 3 && NumHours < 5) {
		return EEDayNightSchedule::LateNight;
	}
	else if (NumHours >= 5 && NumHours < 9) {
		return EEDayNightSchedule::Morning;
	}
	else if (NumHours >= 9 && NumHours < 11) {
		return EEDayNightSchedule::LateMorning;
	}
	else if (NumHours >= 11 && NumHours < 15) {
		return EEDayNightSchedule::Day;
	}
	else if (NumHours >= 15 && NumHours < 17) {
		return EEDayNightSchedule::LateDay;
	}
	else if (NumHours >= 17 && NumHours < 21) {
		return EEDayNightSchedule::Evening;
	}
	else if (NumHours >= 21 && NumHours < 23) {
		return EEDayNightSchedule::LateEvening;
	}
	else {
		return EEDayNightSchedule::Night;
	}
}


float AAccurateDayNightStateBase::GetGameCurrentDaySeconds() {
	return FullDateTime.GetTimeOfDay().GetTotalSeconds();
}

float AAccurateDayNightStateBase::GetGameSolarTime() {
	float result = FullDateTime.GetHour();
	result += FullDateTime.GetMinute() / NUM_MINUTES;
	result += FullDateTime.GetSecond() / NUM_SECONDS;
	result += FullDateTime.GetMillisecond() / NUM_MILISECONDS;
	return result;
}

void AAccurateDayNightStateBase::Tick(float DeltaTime)
{
	float DeltaTimeMyltiply = DeltaTime * GetTimeMuliplier();
	TimePasses += FTimespan::FromSeconds(DeltaTimeMyltiply);

	int32 PreviousHour = FullDateTime.GetHour();
	int32 PreviousMinutes = PreviousHour * 60 + FullDateTime.GetMinute();;
	int32 PreviousDay = FullDateTime.GetDay();

	FullDateTime = GameStartDateTime;
	FullDateTime += TimePasses;

	int32 NumHour = FullDateTime.GetHour();
	int32 NumMinutes = NumHour * 60 + FullDateTime.GetMinute();
	int32 Day = FullDateTime.GetDay();

	if (PreviousMinutes != NumMinutes && bWithDayNightCycle) {

		TEnumAsByte<EEDayNightCicle> ENewDayNightCycle = DefineDayNightCycle(NumMinutes);

		if (ENewDayNightCycle != EDayNightCycle) {
			EDayNightCycle = ENewDayNightCycle;
			OnDayNightCycleChangeDelegate.Broadcast(EDayNightCycle);
		}
	}

	if (NumHour != PreviousHour) {

		TEnumAsByte<EEDayNightSchedule> ENewDayNightSchedule = DefineDayNightSchedule(NumHour);

		if (EDayNightSchedule != ENewDayNightSchedule) {
			EDayNightSchedule = ENewDayNightSchedule;
			OnDayNightScheduleChangeDelegate.Broadcast(EDayNightSchedule);
		}
	}

	if (Day != PreviousDay)
	{
		OnNewDayDelegate.Broadcast();

		//FString SunriseTimeStr = SunriseTime.ToString(TEXT("%h.%m"));
		//UE_LOG(LogTemp, Warning, TEXT("SunriseTime: %s"), *SunriseTimeStr);

		//FString SunsetTimeStr = SunsetTime.ToString(TEXT("%h.%m"));
		//UE_LOG(LogTemp, Warning, TEXT("SunsetTime: %s"), *SunsetTimeStr);
	}

	OnGameStateTickDelegate.Broadcast(FullDateTime);

	//FString NewDateTimeStr = FullDateTime.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("FullDateTime: %s"), *NewDateTimeStr);

	//FString FullDateStr = FullDateTime.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("FullDate: %s"), *FullDateStr);
}

void AAccurateDayNightStateBase::SetDayNightTransitionTime(int32 Hour, int32 Minute)
{
	float Value = (float)Minute + Hour * 60.0f;
	if (Value >= 120.0f) {
		UE_LOG(LogTemp, Warning, TEXT("No effect function call: SetDayNightTransitionTime. Day/Night transition time can not be more than 120 minutes. "));
		return;
	}

	DayNightTransitionTime = FTimespan::FromMinutes(Value);
}

FTimespan AAccurateDayNightStateBase::GetDayNightTransitionTime() {
	return DayNightTransitionTime;
}