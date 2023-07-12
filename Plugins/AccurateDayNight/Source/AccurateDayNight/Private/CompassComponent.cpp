// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassComponent.h"

// Sets default values for this component's properties
UCompassComponent::UCompassComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	CompassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompassMesh"));

	CompassMesh->SetMobility(EComponentMobility::Movable);
	CompassMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	CompassMesh->OverrideMaterials = TArray<UMaterialInterface*>();
	CompassMesh->OverrideMaterials.Reserve(1);
	CompassMesh->OverrideMaterials.Add(nullptr);
	CompassMesh->bVisibleInReflectionCaptures = false;
	CompassMesh->bVisibleInRayTracing = false;
	CompassMesh->CastShadow = false;
	CompassMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));

	FRotator CompassMeshRotation = FRotator(0.000000, 90.000000, 0.000000);

	CompassMesh->SetRelativeRotation(CompassMeshRotation.Quaternion());
	CompassMesh->SetHiddenInGame(true);
	CompassMesh->bIsEditorOnly = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CMPMesh(TEXT("StaticMesh'/SunPosition/Editor/SM_Compass.SM_Compass'"));

	if (CMPMesh.Succeeded())
	{
		CompassMesh->SetStaticMesh(CMPMesh.Object);
	}
}

void  UCompassComponent::SetMeshRotation(const FQuat& CompassQuaternion)
{
	if (::IsValid(CompassMesh))
	{
		CompassMesh->SetWorldRotation(CompassQuaternion);
	}
}


