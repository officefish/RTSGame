// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CompassComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACCURATEDAYNIGHT_API UCompassComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCompassComponent();

public:
	/* Components */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true", Category = "Default", OverrideNativeName = "CompassMesh"))
		UStaticMeshComponent* CompassMesh;

public:

	void SetMeshRotation(const FQuat& CompassQuaternion);
	
};
