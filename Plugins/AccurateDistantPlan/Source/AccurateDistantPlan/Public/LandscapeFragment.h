// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"

#include "Engine/Texture2D.h"

#include "AccurateDayNightStateBase.h"

#include "LandscapeFragment.generated.h"

USTRUCT(BlueprintType)
struct FLandscapeFragmentRules
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UMaterialInterface* SpringMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UMaterialInterface* SummerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UMaterialInterface* AutumnMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UMaterialInterface* WinterMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "AccurateDistantPlan", OverrideNativeName = "LandscapeMaterial"))
		UMaterialInstanceDynamic* LandscapeMaterial;

	/* Textures. Base Colors */
	
	/* Spring */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_BC_Red_1_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_BC_Red_2_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_BC_Green_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_BC_Blue_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_BC_Snow_T;

	/* Summer */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_BC_Red_1_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_BC_Red_2_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_BC_Green_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_BC_Blue_T;

	/* Autumn */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_BC_Red_1_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_BC_Red_2_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_BC_Green_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_BC_Blue_T;

	/* Winter */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Winter_BC_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Winter_Snow_T;


	/* Textures. Normals */

	/* Spring */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_Normal_Red_1_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_Normal_Red_2_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_Normal_Green_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_Normal_Blue_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Spring_Normal_Snow_T;

	/* Summer */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_Normal_Red_1_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_Normal_Red_2_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_Normal_Green_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Summer_Normal_Blue_T;

	/* Autumn */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_Normal_Red_1_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_Normal_Red_2_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_Normal_Green_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Autumn_Normal_Blue_T;

	/* Winter */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Winter_BC_Normal_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Winter_Snow_Normal_T;

	// Special Textures
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Main_Normal_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Splat_T;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture2D* Winter_Splat_T;


};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACCURATEDISTANTPLAN_API ULandscapeFragment : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULandscapeFragment();

	/* Functions */
	UFUNCTION(BlueprintCallable)
		void SetRules(FLandscapeFragmentRules Rules);

	UFUNCTION(BlueprintCallable)
		void SetLandscapeScale(float LandscapeScale);

	UFUNCTION(BlueprintCallable)
		void ChangeSeason(TEnumAsByte<EEDemiSeason> Season);

	UFUNCTION(BlueprintCallable)
		void SmoothChangeSeason(float Delta);

public:

	/**
	 Components
	*/

	/*UPROPERTY(BlueprintReadWrite, NonTransactional, meta = (Category = "AccurateDistantPlan", OverrideNativeName = "Scene"))
		USceneComponent* Scene;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Category = "AccurateDistantPlan", OverrideNativeName = "LandscapeMaterial"))
		UMaterialInstanceDynamic* LandscapeMaterial;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	FLandscapeFragmentRules LandscapeRules;
		
};

