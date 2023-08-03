// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeFragment.h"

// Sets default values for this component's properties
ULandscapeFragment::ULandscapeFragment()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LandscapeFragmentMesh"));

	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MeshComponent->SetWorldScale3D(FVector(1.0f));

	MeshComponent->OverrideMaterials = TArray<UMaterialInterface*>();
	MeshComponent->OverrideMaterials.Add(nullptr);

	MeshComponent->SetCollisionProfileName(FName(TEXT("NoCollision")));

	MeshComponent->bVisibleInRayTracing = false;
	MeshComponent->bReceivesDecals = false;
	MeshComponent->bAffectDynamicIndirectLighting = false;

	//
	MeshComponent->CastShadow = true;
	//
	MeshComponent->bCastDynamicShadow = true;

	MeshComponent->bCastStaticShadow = false;
	MeshComponent->BodyInstance.bAutoWeld = false;
	MeshComponent->SetGenerateOverlapEvents(false);

	
	//if (WinterMaterialFound.Succeeded())
	//{
		//WinterMaterial = WinterMaterialFound.Object;
	//}

	FLandscapeFragmentRules EmptyRules{};
	LandscapeRules = EmptyRules;
}


// Called when the game starts
void ULandscapeFragment::BeginPlay()
{
	Super::BeginPlay();
	
}


void ULandscapeFragment::SetRules(FLandscapeFragmentRules Rules)
{

	if (Rules.StaticMesh != nullptr) 
	{
		MeshComponent->SetStaticMesh(Rules.StaticMesh);
		MeshComponent->RegisterComponent();
	}

	LandscapeRules = Rules;
	
	SetRulesAdditions();

}

void ULandscapeFragment::SetLandscapeScale(float LandscapeScale)
{
	MeshComponent->SetWorldScale3D(FVector(LandscapeScale));
}


