// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyActor.h"

// Sets default values
ASkyActor::ASkyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup default values for class fields
	SetupDefaults();

	// Construct Subobjects
	ConstructSubObjects();

	// Setup Subobjects
	SetupSubObjects();

}


void ASkyActor::SetupDefaults()
{

	//SunPositionRules Config
	FSunPositionData SunPositionData;
	SunPositionData.Elevation = 0.0f;
	SunPositionData.CorrectedElevation = 0.0f;
	SunPositionData.Azimuth = 0.0f;

	FSunPositionRules MakeSP_Rules;
	MakeSP_Rules.NorthOffset = 0.0f;
	MakeSP_Rules.Latitude = 45.0f;
	MakeSP_Rules.Longitude = -73.0f;
	MakeSP_Rules.TimeZone = -5.0f;
	MakeSP_Rules.Data = SunPositionData;

	SunPositionRules = MakeSP_Rules;

	// DayLightSavingsTime Config
	FDayLightSavingsTime MakeDSTS;

	MakeDSTS.bEnable = true;
	MakeDSTS.StartMonth = 3;
	MakeDSTS.StartDay = 10;
	MakeDSTS.EndMonth = 11;
	MakeDSTS.EndDay = 3;
	MakeDSTS.SwitchHours = 2;

	DayLightSavingsTime = MakeDSTS;

	/* Moon rules */
	FMoonRules MoonMake;
	MoonMake.Moon = true;
	MoonMake.MoonEnable = 1.0f;
	MoonMake.MoonLightStrength = 0.01f;
	MoonMake.MoonDistance = 0.0f;
	MoonMake.MoonSize = 0.005f;
	MoonMake.MaxStarsBrightness = 3.5f;

	MoonRules = MoonMake;

	/* DayNightCycleRules */
	FDayNightCycleRules Make;
	FDateTime gw_sst = FDateTime(2009, 1, 20, 18, 0, 0, 0);
	FTimespan rwtigt = FTimespan(0, 1, 0);
	Make.RealWorld_StaticStartTime = FDateTime::UtcNow();
	Make.GameWorld_StaticStartTime = gw_sst;
	Make.RealWorldTimeInGameTime = rwtigt;
	DayNightRules = Make;

	AnimationBeginDateTime = FDateTime{};
	AnimationTotalSeconds = 1;

	bFirstTick = true;

	bWithDay = false;
	bWithNight = false;
	bOnNightBegin = false;
	bOnDayBegin = false;

}

void ASkyActor::ConstructSubObjects()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));

	SunComponent = CreateDefaultSubobject<USunComponent>(TEXT("SunComponent"));

	MoonComponent = CreateDefaultSubobject<UMoonComponent>(TEXT("MoonComponent"));

	ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));

	CompassComponent = CreateDefaultSubobject<UCompassComponent>(TEXT("CompassComponent"));

	RootComponent = Scene;
}

void ASkyActor::SetupSubObjects()
{
	Scene->CreationMethod = EComponentCreationMethod::Native;
	Scene->Mobility = EComponentMobility::Type::Static;

	//Fog
	FLinearColor albedofogcolor = FLinearColor(ALBEDO_COLOR, ALBEDO_COLOR, ALBEDO_COLOR, 1.0f);

	ExponentialHeightFog->FogDensity = 0.0005f;
	ExponentialHeightFog->FogMaxOpacity = 0.3f;
	ExponentialHeightFog->SetVolumetricFog(true);
	ExponentialHeightFog->VolumetricFogScatteringDistribution = 0.9f;
	ExponentialHeightFog->VolumetricFogAlbedo = albedofogcolor.ToFColor(false);
	ExponentialHeightFog->VolumetricFogExtinctionScale = 15.0f;

	//Sun
	SunComponent->SetMobility(EComponentMobility::Movable);
	SunComponent->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);

	//Skylight
	SkyLight->SetMobility(EComponentMobility::Movable);
	SkyLight->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);

	SkyLight->SourceType = ESkyLightSourceType::SLS_CapturedScene;
	SkyLight->SamplesPerPixel = 2;

	//SkyAtmosphere
	SkyAtmosphere->SetMobility(EComponentMobility::Movable);
	SkyAtmosphere->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);

	//CompassMesh
	CompassComponent->SetMobility(EComponentMobility::Movable);
	CompassComponent->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);

	//Moon
	MoonComponent->SetMobility(EComponentMobility::Movable);
	MoonComponent->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
	SetMoonRules(MoonRules);
}

ACultureGameStateBase* ASkyActor::GetGameState() const {
	return ::IsValid(GetWorld()) ?
		GetWorld()->GetGameState<ACultureGameStateBase>()
		: nullptr;
}

// Called when the game starts or when spawned
void ASkyActor::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetGameState();

	if (::IsValid(GameState)) {
		
		GameState->OnGameStateTickDelegate.AddDynamic(this, &ASkyActor::OnGameStateTick);
		GameState->OnDayNightCycleChangeDelegate.AddDynamic(this, &ASkyActor::OnDayNightCycleChange);
		GameState->OnNewDayDelegate.AddDynamic(this, &ASkyActor::OnNextDay);

		ECurrentDayNightCicle = GameState->EDayNightCycle;

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GameState is not detected"));
	}
}

void ASkyActor::UpdateGameStateData()
{
	//UE_LOG(LogTemp, Warning, TEXT("ASkyActor::UpdateGameStateData()"));

	if (::IsValid(GameState)) {
		GameState->SunriseTime = SunPositionRules.Data.SunriseTime;
		GameState->SunsetTime = SunPositionRules.Data.SunsetTime;
	}
}

void ASkyActor::OnNextDay()
{
	UpdateGameStateData();
}

void ASkyActor::OnDayNightCycleChange(TEnumAsByte<EEDayNightCicle> EDayNightCicle)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnDayNightCicleChange"));

	ECurrentDayNightCicle = EDayNightCicle;
	if (::IsValid(GameState)) {
		AnimationTotalSeconds = GameState->DayNightTransitionTime.GetTotalSeconds();
		AnimationBeginDateTime = GameState->FullDateTime;
	}

}

void ASkyActor::OnGameStateTick(FDateTime CurrentDateTime)
{
	
	TickDateTime = CurrentDateTime;

	FRotator MainRotator;
	GetCurrentRotator(MainRotator, SunPositionRules);

	if (bWithDay) {
		UpdateSun(MainRotator);
		UpdateSky();
	}
	
	if(bWithNight) {
		FRotator MoonRotator;
		MoonRotator.Roll = MainRotator.Roll;
		MoonRotator.Yaw = MainRotator.Yaw;
		MoonRotator.Pitch = MainRotator.Pitch - 180;
		UpdateMoon(MoonRotator);
	}

	UpdateCompass(FRotator(0, SunPositionRules.NorthOffset + 90, 0).Quaternion());

	SmoothUpdate();

	/* Set Sunset/Sunrise Time for GameState first time */
	if (bFirstTick) {
		bFirstTick = false;
		UpdateGameStateData();
	}
}

void ASkyActor::GetCurrentRotator(FRotator& CurrentRotator, FSunPositionRules& Rules)
{
	bool bIsDST = false;
	FSunPositionData SunPositionDt;

	bIsDST = IsDST(DayLightSavingsTime);

	USunPositionFunctionLibrary::GetSunPosition(
		Rules.Latitude, Rules.Longitude, Rules.TimeZone,
		bIsDST,
		TickDateTime.GetYear(), TickDateTime.GetMonth(), TickDateTime.GetDay(),
		TickDateTime.GetHour(), TickDateTime.GetMinute(), TickDateTime.GetSecond(),
		SunPositionDt);

	Rules.Data.Elevation = SunPositionDt.Elevation;
	Rules.Data.CorrectedElevation = SunPositionDt.CorrectedElevation;
	Rules.Data.Azimuth = SunPositionDt.Azimuth;
	Rules.Data.SunriseTime = SunPositionDt.SunriseTime;
	Rules.Data.SunsetTime = SunPositionDt.SunsetTime;
	Rules.Data.SolarNoon = SunPositionDt.SolarNoon;

	CurrentRotator = FRotator(Rules.Data.CorrectedElevation, Rules.Data.Azimuth + Rules.NorthOffset, 0.0f);
}

void ASkyActor::UpdateSky()
{
	if (SureSkyLight())
	{
		SkyLight->USkyLightComponent::RecaptureSky();
	}
}


void ASkyActor::UpdateSun(const FRotator& SunRotator)
{
	if (SureSunComponent())
	{
		SunComponent->SetSunRotation(SunRotator);
	}
}

void ASkyActor::UpdateMoon(const FRotator& MoonRotator)
{
	if (SureMoonComponent())
	{
		MoonComponent->SetMoonRotation(MoonRotator);
		MoonComponent->MoveMoon();
	}
}

void ASkyActor::UpdateCompass(const FQuat& CompassQuaternion)
{
	if (SureCompassComponent())
	{
		CompassComponent->SetMeshRotation(CompassQuaternion);
	}
}


void ASkyActor::SmoothUpdate()
{
	float Modifier = GetSmoothModifier();
	float InverseModifier = 1 - Modifier;

	switch (ECurrentDayNightCicle)
	{
	case EEDayNightCicle::BeforeSunsetStart:
	{
		bWithDay = true;
		bWithNight = true;
		OnBeforeSunset(Modifier, InverseModifier);
		break;
	}
	case EEDayNightCicle::SunsetStart:
	{
		bWithDay = true;
		bWithNight = true;
		OnSunset(Modifier, InverseModifier);
		break;
	}
	case EEDayNightCicle::NightStart:
	{
		bWithDay = false;
		bWithNight = true;
		bOnNightBegin = true;
		OnNight(Modifier, InverseModifier);
		break;
	}
	case EEDayNightCicle::BeforeSunriseStart:
	{
		bWithDay = true;
		bWithNight = true;
		OnBeforeSunrise(Modifier, InverseModifier);
		break;
	}
	case EEDayNightCicle::SunriseStart:
	{
		bWithDay = true;
		bWithNight = true;
		OnSunrise(Modifier, InverseModifier);
		break;
	}
	case EEDayNightCicle::DayStart:
	{
		bWithDay = true;
		bWithNight = false;
		bOnDayBegin = true;
		OnDay(Modifier, InverseModifier);
		break;
	}
	}
}

float ASkyActor::GetSmoothModifier() const
{
	FTimespan TickTimespan = TickDateTime - AnimationBeginDateTime;

	float TickSeconds = TickTimespan.GetTotalSeconds();
	float Modifier = TickSeconds / AnimationTotalSeconds;

	return FMath::Clamp(Modifier, 0.0f, 1.0f);
}


void ASkyActor::SetMoonRules(FMoonRules MoonRulesInput) {
	if (SureMoonComponent())
	{
		MoonComponent->SetMoonRules(MoonRulesInput);
	}
}

FTimespan ASkyActor::GetSunsetTime()
{
	return SunPositionRules.Data.SunsetTime;

}

FTimespan ASkyActor::GetSunriseTime()
{
	return SunPositionRules.Data.SunriseTime;
}

bool ASkyActor::IsDST(FDayLightSavingsTime DSTStruct)
{
	FDateTime Time_A = TickDateTime;
	FDateTime Time_B = FDateTime(TickDateTime.GetYear(), DSTStruct.StartMonth, DSTStruct.StartDay, DSTStruct.SwitchHours, 0, 0, 0);
	FDateTime Time_C = FDateTime(TickDateTime.GetYear(), DSTStruct.EndMonth, DSTStruct.EndMonth, DSTStruct.SwitchHours, 0, 0, 0);

	if (Time_A >= Time_B && Time_A <= Time_C && DSTStruct.bEnable)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// DayNight Cycle Change Animations
void ASkyActor::OnBeforeSunset(float Modifier, float InverseModifier)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnBeforeSunset"));
	ShowFog();
	SetFogAlpha(Modifier * 0.5);

	if (SureMoonComponent())
	{
		MoonComponent->SetStarsBrightness(Modifier);
		MoonComponent->SetMoonIntensity(MIN_VALUE);
		MoonComponent->SetMoonEnable(MIN_VALUE);
	}

	if (SureSunComponent())
	{
		SunComponent->SetIntensity(MAX_VALUE);
	}
}

void ASkyActor::OnSunset(float Modifier, float InverseModifier)
{
	ShowFog();
	SetFogAlpha(Modifier * 0.5 + 0.5);

	if (SureMoonComponent())
	{
		MoonComponent->SetStarsBrightness(MAX_VALUE);
		MoonComponent->SetMoonIntensity(Modifier);
		MoonComponent->SetMoonEnable(Modifier);
	}

	if (SureSunComponent())
	{
		SunComponent->SetIntensity(InverseModifier);
	}
}

void ASkyActor::OnNight(float Modifier, float InverseModifier)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnNight"));
	if (!bOnNightBegin) return;

	ShowFog();

	if (SureMoonComponent())
	{
		MoonComponent->SetStarsBrightness(MAX_VALUE);
		MoonComponent->SetMoonIntensity(MAX_VALUE);
		MoonComponent->SetMoonEnable(MAX_VALUE);
	}

	if (SureSunComponent())
	{
		SunComponent->SetIntensity(MIN_VALUE);
	}

	bOnNightBegin = false;
}

void ASkyActor::OnBeforeSunrise(float Modifier, float InverseModifier)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnBeforeSunrise"));

	SetFogAlpha(InverseModifier * 0.5f + 0.5f);
	ShowFog();

	if (SureMoonComponent())
	{
		MoonComponent->SetStarsBrightness(InverseModifier);
		MoonComponent->SetMoonIntensity(InverseModifier);
		MoonComponent->SetMoonEnable(MAX_VALUE);
	}

	if (SureSunComponent())
	{
		SunComponent->SetIntensity(Modifier);
	}

}

void ASkyActor::OnSunrise(float Modifier, float InverseModifier)
{
	
	SetFogAlpha(0.5f * InverseModifier);
	ShowFog();

	if (SureMoonComponent())
	{
		MoonComponent->SetStarsBrightness(MIN_VALUE);
		MoonComponent->SetMoonIntensity(MIN_VALUE);
		MoonComponent->SetMoonEnable(InverseModifier);
	}

	if (SureSunComponent())
	{
		SunComponent->SetIntensity(MAX_VALUE);
	}
}

void ASkyActor::OnDay(float Modifier, float InverseModifier)
{
	if (!bOnDayBegin) return;

	SetFogAlpha(MIN_VALUE);
	HideFog();

	if (SureMoonComponent())
	{
		MoonComponent->SetStarsBrightness(MIN_VALUE);
		MoonComponent->SetMoonIntensity(MIN_VALUE);
		MoonComponent->SetMoonEnable(MIN_VALUE);
	}

	if (SureSunComponent())
	{
		SunComponent->SetIntensity(MAX_VALUE);
	}

	bOnDayBegin = false;
}


void ASkyActor::ShowFog()
{
	if (SureFog())
	{
		ExponentialHeightFog->SetHiddenInGame(false);
	}
}

void ASkyActor::HideFog()
{
	if (SureFog())
	{
		ExponentialHeightFog->SetHiddenInGame(true);
	}
}

void ASkyActor::SetFogAlpha(float FogAlpha) {

	if (SureFog())
	{
		ExponentialHeightFog->SetFogDensity(0.0005f * FogAlpha);
		ExponentialHeightFog->SetFogMaxOpacity(0.2f * FogAlpha);
	}
}