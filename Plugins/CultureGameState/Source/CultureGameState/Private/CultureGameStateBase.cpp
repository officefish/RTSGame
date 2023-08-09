// Fill out your copyright notice in the Description page of Project Settings.

#include "CultureGameStateBase.h"

ACultureGameStateBase::ACultureGameStateBase()
	: AGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bWithSlowMotion = false;
	bWithX50Time = false;
	bWithX500Time = false;
	bWithNextDay = false;

	bWithRealTime = true;
	EGameSpeed = EEGameSpeed::GameTime;

	// Demiseason period in days
	DemiSeasonNumDays = FTimespan::FromDays(14.0f);

	// Early/late start demi season incontancy
	DemiSeasonInconstancy = FTimespan::FromDays(7.0f);

	SetGameTimeMultiplier(60.0f);

	SetGameStartDateTime(FDateTime::UtcNow());

	SunriseTime = FTimespan::FromHours(8.0f);
	SunsetTime = FTimespan::FromHours(21.0f);

	DayNightTransitionTime = FTimespan::FromMinutes(35.0f);

	bWithDayNightCycle = true;

}

//
FDemiSeasonKeyFrames ACultureGameStateBase::GetCurrentDemiSeasonKeyFrames(
	const FDateTime& CurrentDateTime,
	const FTimespan& Inconstancy,
	const FTimespan& NumDays
)
{
	FDemiSeasonKeyFrames CurrentSeasonKeyFrames;

	/* Generate random season shifts */
	int32 RandomSpringShift = FMath::RandRange(0, Inconstancy.GetDays());
	int32 RandomSummerShift = FMath::RandRange(0, Inconstancy.GetDays());
	int32 RandomAutumnShift = FMath::RandRange(0, Inconstancy.GetDays());
	int32 RandomWinterShift = FMath::RandRange(0, Inconstancy.GetDays());

	/* Get Season Bases */
	int32 SpringStart = 31 + 28;
	int32 SummerStart = SpringStart + 91;
	int32 AutumnStart = SummerStart + 91;
	int32 WinterStart = AutumnStart + 91;

	FTimespan SpringStartTimespan = FTimespan::FromDays(
		SpringStart - Inconstancy.GetDays() / 2 + RandomSpringShift);

	FTimespan SummerStartTimespan = FTimespan::FromDays(
		SummerStart - Inconstancy.GetDays() / 2 + RandomSummerShift);

	FTimespan AutumnStartTimespan = FTimespan::FromDays(
		AutumnStart - Inconstancy.GetDays() / 2 + RandomAutumnShift);

	FTimespan WinterStartTimespan = FTimespan::FromDays(
		WinterStart - Inconstancy.GetDays() / 2 + RandomWinterShift);

	/*Make demi season start frames */

	/* ... Spring frames */
	FDateTime SpringStartDateTime{ CurrentDateTime.GetYear(), 1, 1 };
	SpringStartDateTime += SpringStartTimespan;

	CurrentSeasonKeyFrames.SpringStartTime = SpringStartDateTime;
	CurrentSeasonKeyFrames.SpringTime = SpringStartDateTime + NumDays;

	/* ... Summer frames */
	FDateTime SummerStartDateTime{ CurrentDateTime.GetYear(), 1, 1 };
	SummerStartDateTime += SummerStartTimespan;

	CurrentSeasonKeyFrames.SummerStartTime = SummerStartDateTime;
	CurrentSeasonKeyFrames.SummerTime = SummerStartDateTime + NumDays;

	/* ... Autumn frames */
	FDateTime AutumnStartDateTime{ CurrentDateTime.GetYear(), 1, 1 };
	AutumnStartDateTime += AutumnStartTimespan;

	CurrentSeasonKeyFrames.AutumnStartTime = AutumnStartDateTime;
	CurrentSeasonKeyFrames.AutumnTime = AutumnStartDateTime + NumDays;

	/* ... Winter frames */
	FDateTime WinterStartDateTime{ CurrentDateTime.GetYear(), 1, 1 };
	WinterStartDateTime += WinterStartTimespan;

	CurrentSeasonKeyFrames.WinterStartTime = WinterStartDateTime;
	CurrentSeasonKeyFrames.WinterTime = WinterStartDateTime + NumDays;

	/* Logging */

	//FString SpringStartTimeStr = CurrentSeasonKeyFrames.SpringStartTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Spring StartTime: %s"), *SpringStartTimeStr);

	//FString SpringTimeStr = CurrentSeasonKeyFrames.SpringTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Spring Time: %s"), *SpringTimeStr);

	//FString SummerStartTimeStr = CurrentSeasonKeyFrames.SummerStartTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Summer StartTime: %s"), *SummerStartTimeStr);

	//FString SummerTimeStr = CurrentSeasonKeyFrames.SummerTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Summer Time: %s"), *SummerTimeStr);

	//FString AutumnStartTimeStr = CurrentSeasonKeyFrames.AutumnStartTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Autumn StartTime: %s"), *AutumnStartTimeStr);

	//FString AutumnTimeStr = CurrentSeasonKeyFrames.AutumnTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Autumn Time: %s"), *AutumnTimeStr);

	//FString WinterStartTimeStr = CurrentSeasonKeyFrames.WinterStartTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Winter StartTime: %s"), *WinterStartTimeStr);

	//FString WinterTimeStr = CurrentSeasonKeyFrames.WinterTime
	//	.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("Winter Time: %s"), *WinterTimeStr);

	return CurrentSeasonKeyFrames;

}

//
void ACultureGameStateBase::SetGameSpeed(TEnumAsByte<EEGameSpeed> EGameSpeedStatus)
{
	EGameSpeed = EGameSpeedStatus;
	OnGameSpeedChangeDelegate.Broadcast(EGameSpeed);
}
//

void ACultureGameStateBase::SetGameTimeMultiplier(float Value)
{
	GameTimeMultiplier = Value;
	GameTimeMultiplierX2 = Value * 2;
	GameTimeMultiplierX5 = Value * 5;	
	GameTimeMultiplierX50 = Value * 50;
	GameTimeMultiplierX500 = Value * 500;
}

//
float ACultureGameStateBase::GetGameTimeMultiplier()
{
	return GameTimeMultiplier;
}

//
void ACultureGameStateBase::SetGameStartDateTime(const FDateTime& DateTime)
{
	GameStartDateTime = DateTime;

	int NumHours = GameStartDatetime.GetHour();
	EDayNightCycle = DefineDayNightCycle(NumHours);
	TimePasses = FTimespan(0, 0, 0, 0);
	FullDateTime = DateTime;
	TotalSeconds = 0.0f;

	/* Define Schedule */
	int NumHour = GameStartDateTime.GetHour();
	ValidateDayNightSchedule(NumHour);

	/* Recalculate Season KeyFrames */
	SeasonKeyFrames = CalculateSeasonKeyFrames(GameStartDateTime);

	/* Define Demiseason*/
	ValidateDemiSeason();

}

//
FDemiSeasonKeyFrames ACultureGameStateBase::CalculateSeasonKeyFrames(FDateTime StartDateTime)
{
	// Calculate Demi season keyframes
	FDemiSeasonKeyFrames KeyFrames = GetCurrentDemiSeasonKeyFrames(
		StartDateTime, DemiSeasonInconstancy, DemiSeasonNumDays);

	return KeyFrames;
}

//
float ACultureGameStateBase::GetTimeMuliplier()
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
	else if (EGameSpeed == EEGameSpeed::GameTimeX500) {
		return GameTimeMultiplierX500;
	} 
	
	return 0.0f;
	
}

//
void ACultureGameStateBase::GameSpeedUp()
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
		else if (bWithX500Time) {
			EGameSpeed = EEGameSpeed::GameTimeX500;
		}
		else if (bWithNextDay) {
			EGameSpeed = EEGameSpeed::GameTimeNextDay;
		}
		else {
			bNeedBroadcast = false;
		}
	}

	else if (EGameSpeed == EEGameSpeed::GameTimeX50) {
		
		if (bWithX500Time) {
			EGameSpeed = EEGameSpeed::GameTimeX500;
		}
		else if (bWithNextDay) {
			EGameSpeed = EEGameSpeed::GameTimeNextDay;
		}
		else {
			bNeedBroadcast = false;
		}
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX500) {
		if (bWithNextDay) {
			EGameSpeed = EEGameSpeed::GameTimeNextDay;
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

//
void ACultureGameStateBase::GameSpeedDown()
{
	bool bNeedBroadcast = true;

	if (EGameSpeed == EEGameSpeed::GameTimeNextDay) {
		if (bWithX500Time) {
			EGameSpeed = EEGameSpeed::GameTimeX500;
		} else if (bWithX50Time) {
			EGameSpeed = EEGameSpeed::GameTimeX50;
		}
		else {
			EGameSpeed = EEGameSpeed::GameTimeX5;
		}
	}
	else if (EGameSpeed == EEGameSpeed::GameTimeX500) {
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

//
TEnumAsByte<EEDayNightCicle> ACultureGameStateBase::DefineDayNightCycle(const int32& NumMinutes)
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

//
TEnumAsByte<EEDayNightSchedule> ACultureGameStateBase::DefineDayNightSchedule(const int32& NumHours)
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

//
TEnumAsByte<EEDemiSeason> ACultureGameStateBase::DefineDemiSeason(
	const FDateTime& CurrentDateTime, const FDemiSeasonKeyFrames& KeyFrames)
{
	//UE_LOG()

	int32 NumDays = CurrentDateTime.GetDayOfYear();

	if (NumDays < KeyFrames.SpringStartTime.GetDayOfYear())
	{
		return EEDemiSeason::Winter;
	}
	else if (NumDays >= KeyFrames.SpringStartTime.GetDayOfYear()
		&& NumDays < KeyFrames.SpringTime.GetDayOfYear())
	{
		return EEDemiSeason::WinterToSpring;
	}
	else if (NumDays >= KeyFrames.SpringTime.GetDayOfYear()
		&& NumDays < KeyFrames.SummerStartTime.GetDayOfYear())
	{
		return EEDemiSeason::Spring;
	}
	else if (NumDays >= KeyFrames.SummerStartTime.GetDayOfYear()
		&& NumDays < KeyFrames.SummerTime.GetDayOfYear())
	{
		return EEDemiSeason::SpringToSummer;
	}
	else if (NumDays >= KeyFrames.SummerTime.GetDayOfYear()
		&& NumDays < KeyFrames.AutumnStartTime.GetDayOfYear())
	{
		return EEDemiSeason::Summer;
	}
	else if (NumDays >= KeyFrames.AutumnStartTime.GetDayOfYear()
		&& NumDays < KeyFrames.AutumnTime.GetDayOfYear())
	{
		return EEDemiSeason::SummerToAutumn;
	}
	else if (NumDays >= KeyFrames.AutumnTime.GetDayOfYear()
		&& NumDays < KeyFrames.WinterStartTime.GetDayOfYear())
	{
		return EEDemiSeason::Autumn;
	}
	else if (NumDays >= KeyFrames.WinterStartTime.GetDayOfYear()
		&& NumDays < KeyFrames.WinterTime.GetDayOfYear())
	{
		return EEDemiSeason::AutumnToWinter;
	}
	else {
		return EEDemiSeason::Winter;
	}
}

//
float ACultureGameStateBase::DefineDemiSeasonDelta(
	TEnumAsByte<EEDemiSeason> CurrentSeason,
	const FDemiSeasonKeyFrames& CurrentSeasonKeyFrames,
	const FDateTime& CurrentDateTime)
{
	switch (CurrentSeason) {
	case EEDemiSeason::Spring:
	{
		return 1.0;
	}
	case EEDemiSeason::Summer:
	{
		return 1.0;
	}
	case EEDemiSeason::Autumn:
	{
		return 1.0;
	}
	case EEDemiSeason::Winter:
	{
		return 1.0;
	}
	case EEDemiSeason::WinterToSpring:
	{
		float Divisible = (CurrentSeasonKeyFrames.SpringTime - CurrentSeasonKeyFrames.SpringStartTime)
			.GetTotalMinutes();

		float Divisor = (CurrentSeasonKeyFrames.SpringTime - CurrentDateTime)
			.GetTotalMinutes();

		return FMath::Clamp(Divisor / Divisible, 0.0f, +1.0f);
	}
	case EEDemiSeason::SpringToSummer:
	{
		float Divisible = (CurrentSeasonKeyFrames.SummerTime - CurrentSeasonKeyFrames.SummerStartTime)
			.GetTotalMinutes();

		float Divisor = (CurrentSeasonKeyFrames.SummerTime - CurrentDateTime)
			.GetTotalMinutes();

		return FMath::Clamp(Divisor / Divisible, 0.0f, +1.0f);
	}
	case EEDemiSeason::SummerToAutumn:
	{
		float Divisible = (CurrentSeasonKeyFrames.AutumnTime - CurrentSeasonKeyFrames.AutumnStartTime)
			.GetTotalMinutes();

		float Divisor = (CurrentSeasonKeyFrames.AutumnTime - CurrentDateTime)
			.GetTotalMinutes();

		return FMath::Clamp(Divisor / Divisible, 0.0f, +1.0f);
	}
	case EEDemiSeason::AutumnToWinter:
	{
		float Divisible = (CurrentSeasonKeyFrames.WinterTime - CurrentSeasonKeyFrames.WinterStartTime)
			.GetTotalMinutes();

		float Divisor = (CurrentSeasonKeyFrames.WinterTime - CurrentDateTime)
			.GetTotalMinutes();

		return FMath::Clamp(Divisor / Divisible, 0.0f, +1.0f);
	}
	default:
		return 0.0f;
	}
}

//
float ACultureGameStateBase::GetGameCurrentDaySeconds() 
{
	return FullDateTime.GetTimeOfDay().GetTotalSeconds();
}

//
float ACultureGameStateBase::GetGameSolarTime() 
{
	float result = FullDateTime.GetHour();
	result += FullDateTime.GetMinute() / NUM_MINUTES;
	result += FullDateTime.GetSecond() / NUM_SECONDS;
	result += FullDateTime.GetMillisecond() / NUM_MILISECONDS;
	return result;
}


//
void ACultureGameStateBase::SetDayNightTransitionTime(int32 Hour, int32 Minute)
{
	float Value = (float)Minute + Hour * 60.0f;
	if (Value >= 120.0f) {
		UE_LOG(LogTemp, Warning, 
			TEXT("No effect function call: SetDayNightTransitionTime. Day/Night transition time can not be more than 120 minutes. "));
		return;
	}

	DayNightTransitionTime = FTimespan::FromMinutes(Value);
}

//
FTimespan ACultureGameStateBase::GetDayNightTransitionTime() 
{
	return DayNightTransitionTime;
}

/* Validations */
void ACultureGameStateBase::ValidateDayNightCircle(int32 NumMinutes)
{
	TEnumAsByte<EEDayNightCicle> ENewDayNightCycle = DefineDayNightCycle(NumMinutes);

	if (ENewDayNightCycle != EDayNightCycle)
	{
		EDayNightCycle = ENewDayNightCycle;
		OnDayNightCycleChangeDelegate.Broadcast(EDayNightCycle);
	}
}


void ACultureGameStateBase::ValidateDayNightSchedule(int32 NumHour)
{
	TEnumAsByte<EEDayNightSchedule> ENewDayNightSchedule = DefineDayNightSchedule(NumHour);

	if (EDayNightSchedule != ENewDayNightSchedule)
	{
		EDayNightSchedule = ENewDayNightSchedule;
		OnDayNightScheduleChangeDelegate.Broadcast(EDayNightSchedule);
	}
}

void ACultureGameStateBase::ValidateDemiSeason()
{

	ECurrentDemiSeason = DefineDemiSeason(FullDateTime, SeasonKeyFrames);
	// From one to zero
	float DemiSeasonDelta = DefineDemiSeasonDelta(ECurrentDemiSeason, SeasonKeyFrames, FullDateTime);
	// From zero to one
	float InverseDelta = 1.0f - DemiSeasonDelta;

	// Broadcast DemiSeason enum and DemiSeason duration delta. 
	OnSeasonChangeDelegate.Broadcast(ECurrentDemiSeason, InverseDelta);
}

void ACultureGameStateBase::NextDayTick()
{
	int32 PreviousYear = FullDateTime.GetYear();

	TimePasses += FTimespan::FromDays(1.0f);
	FullDateTime = GameStartDateTime;
	FullDateTime += TimePasses;

	int32 Year = FullDateTime.GetYear();

	if (Year != PreviousYear)
	{
		SeasonKeyFrames = CalculateSeasonKeyFrames(FullDateTime);
	}

	ValidateDemiSeason();

	/* New day broadcast */
	OnNewDayDelegate.Broadcast();

	/* Tick broadcast */
	OnGameStateTickDelegate.Broadcast(FullDateTime);
}

//
void ACultureGameStateBase::Tick(float DeltaTime)
{
	if (EGameSpeed == EEGameSpeed::GameTimeNextDay)
	{
		NextDayTick();
		return;
	}

	float DeltaTimeMyltiply = DeltaTime * GetTimeMuliplier();
	TimePasses += FTimespan::FromSeconds(DeltaTimeMyltiply);

	int32 PreviousHour = FullDateTime.GetHour();
	int32 PreviousMinutes = PreviousHour * 60 + FullDateTime.GetMinute();;
	int32 PreviousDay = FullDateTime.GetDay();
	int32 PreviousYear = FullDateTime.GetYear();

	FullDateTime = GameStartDateTime;
	FullDateTime += TimePasses;

	int32 NumHour = FullDateTime.GetHour();
	int32 NumMinutes = NumHour * 60 + FullDateTime.GetMinute();
	int32 Day = FullDateTime.GetDay();
	int32 Year = FullDateTime.GetYear();

	if (Year != PreviousYear)
	{
		SeasonKeyFrames = CalculateSeasonKeyFrames(FullDateTime);
	}

	if (PreviousMinutes != NumMinutes && bWithDayNightCycle) 
	{
		ValidateDayNightCircle(NumMinutes);
	}

	if (NumHour != PreviousHour) 
	{
		ValidateDayNightSchedule(NumHour);
		ValidateDemiSeason();
	}

	if (Day != PreviousDay)
	{
		OnNewDayDelegate.Broadcast();

		//FString SunriseTimeStr = SunriseTime.ToString(TEXT("%h.%m"));
		//UE_LOG(LogTemp, Warning, TEXT("SunriseTime: %s"), *SunriseTimeStr);

		//FString SunsetTimeStr = SunsetTime.ToString(TEXT("%h.%m"));
		//UE_LOG(LogTemp, Warning, TEXT("SunsetTime: %s"), *SunsetTimeStr);

		FString FullDateStr = FullDateTime.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
		UE_LOG(LogTemp, Warning, TEXT("FullDate: %s"), *FullDateStr);
	}

	

	OnGameStateTickDelegate.Broadcast(FullDateTime);

	//FString NewDateTimeStr = FullDateTime.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("FullDateTime: %s"), *NewDateTimeStr);

	//FString FullDateStr = FullDateTime.ToString(TEXT("%Y.%m.%d-%H.%M.%S"));
	//UE_LOG(LogTemp, Warning, TEXT("FullDate: %s"), *FullDateStr);
}