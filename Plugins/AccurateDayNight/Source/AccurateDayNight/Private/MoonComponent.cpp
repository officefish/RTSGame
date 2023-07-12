// Fill out your copyright notice in the Description page of Project Settings.


#include "MoonComponent.h"

// Sets default values for this component's properties
UMoonComponent::UMoonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
	SkySphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphereMesh"));

	//MoonLight
	MoonLight->SetMobility(EComponentMobility::Movable);

	MoonLight->DynamicShadowCascades = 5;
	MoonLight->CascadeDistributionExponent = 1.400000f;
	MoonLight->LightSourceAngle = 0.500000f;
	//Comment this one in UE5 Preview2
	MoonLight->bUsedAsAtmosphereSunLight = true;
	//Uncomment this one in UE5 Preview2
	//MoonLight->bAtmosphereSunLight = true;
	MoonLight->Temperature = 4000.000000f;
	MoonLight->bUseTemperature = true;
	MoonLight->Intensity = 0.000000f;

	//SkySphereMesh
	SkySphereMesh->CreationMethod = EComponentCreationMethod::Native;
	SkySphereMesh->SetMobility(EComponentMobility::Static);
	SkySphereMesh->OverrideMaterials = TArray<UMaterialInterface*>();
	SkySphereMesh->OverrideMaterials.Add(nullptr);

	SkySphereMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
	SkySphereMesh->SetRelativeScale3D(FVector(400.000000, 400.000000, 400.000000));
	SkySphereMesh->bVisibleInRayTracing = false;
	SkySphereMesh->bReceivesDecals = false;
	SkySphereMesh->CastShadow = false;
	SkySphereMesh->bAffectDynamicIndirectLighting = false;
	SkySphereMesh->bCastDynamicShadow = false;
	SkySphereMesh->bCastStaticShadow = false;
	SkySphereMesh->BodyInstance.bAutoWeld = false;
	SkySphereMesh->SetGenerateOverlapEvents(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MSHSkySphere(TEXT("StaticMesh'/Engine/EngineSky/SM_SkySphere.SM_SkySphere'"));

	if (MSHSkySphere.Succeeded())
	{
		SkySphereMesh->SetStaticMesh(MSHSkySphere.Object);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Sky sphere mesh found."));
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> X_SphereMaterialInstance(TEXT("MaterialInstanceConstant'/AccurateDayNight/M_NightSky_Inst.M_NightSky_Inst'"));

	if (X_SphereMaterialInstance.Succeeded())
	{
		SkySphereMesh->SetMaterial(0, X_SphereMaterialInstance.Object);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NightSky material instance not found."));
	}

	NightSkyMaterialFound = nullptr;

	static ConstructorHelpers::FObjectFinder<UMaterial> SkyMatFind(TEXT("/AccurateDayNight/M_NightSky.M_NightSky"));

	if (SkyMatFind.Succeeded())
	{
		NightSkyMaterialFound = SkyMatFind.Object;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NightSky material not found."));
	}
}


void UMoonComponent::SetMoonRotation(const FRotator& MoonRotator)
{
	if (::IsValid(MoonLight))
	{
		MoonLight->SetWorldRotation(MoonRotator);
	}
}

void UMoonComponent::SetMoonRules(FMoonRules InputMoonRules) {

	MoonRules = InputMoonRules;

	SetMoonRadius(MoonRules.MoonSize);
	//SetMoonEnable(MoonRules.MoonEnable);
	//SetMoonIntensity(MoonRules.MoonLightStrength);
}

/*
*
*/
void UMoonComponent::MoveMoon()
{
	if (::IsValid(NightSkyMaterial) && ::IsValid(MoonLight))
	{
		FRotator MoonRot = MoonLight->GetComponentRotation();
		FLinearColor Color = FLinearColor(MoonRot.Vector());

		NightSkyMaterial->SetVectorParameterValue(FName(TEXT("MoonSkyRotator")), Color);
	}
}

void UMoonComponent::SetMoonRadius(float MoonRadius)
{
	if (::IsValid(NightSkyMaterial))
	{
		FLinearColor color = FLinearColor(MoonRadius, MoonRadius, MoonRadius, MoonRadius);
		NightSkyMaterial->SetVectorParameterValue(FName(TEXT("MoonRadius")), color);
	}
}

void UMoonComponent::SetMoonEnable(float MoonEnable)
{
	if (::IsValid(NightSkyMaterial))
	{
		NightSkyMaterial->SetScalarParameterValue(FName(TEXT("EnableMoon")), MoonEnable);
	}
}

void UMoonComponent::SetMoonIntensity(float Modifier)
{
	if (::IsValid(MoonLight))
	{
		MoonLight->SetIntensity(MoonRules.MoonLightStrength * Modifier);
	}
}

void  UMoonComponent::SetStarsBrightness(float Modifier)
{
	if (::IsValid(NightSkyMaterial))
	{
		NightSkyMaterial->SetScalarParameterValue(FName(TEXT("StarsBrightness")),
			MoonRules.MaxStarsBrightness * Modifier);
	}
}

// Called when the game starts
void UMoonComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("OnConstruction"));

	if (NightSkyMaterialFound != nullptr && ::IsValid(SkySphereMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeSpereMesh DynamicMaterialInstance created."));
		NightSkyMaterial = SkySphereMesh->CreateDynamicMaterialInstance(0, NightSkyMaterialFound);
	} {
		UE_LOG(LogTemp, Warning, TEXT("SkeSpereMesh use static material."));
	}

	if (::IsValid(MoonLight)) {
		MoonLight->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (::IsValid(SkySphereMesh))
	{
		SkySphereMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

}

