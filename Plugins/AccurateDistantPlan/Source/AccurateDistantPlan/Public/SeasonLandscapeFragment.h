// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialLayersFunctions.h"
#include "Misc/Guid.h"

#include "CultureGameStateBase.h"

#include "LandscapeFragment.h"
#include "SeasonLandscapeFragment.generated.h"

/**
 * 
 */
UCLASS()
class ACCURATEDISTANTPLAN_API USeasonLandscapeFragment : public ULandscapeFragment
{
	GENERATED_BODY()

public:
	USeasonLandscapeFragment();

public:
	UFUNCTION(BlueprintCallable)
		void ChangeSeason(TEnumAsByte<EEDemiSeason> Season);

	UFUNCTION(BlueprintCallable)
		void SmoothChangeSeason(float Delta);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UMaterialInterface* BlendMaterialInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UMaterialInstanceDynamic* BlendMaterialInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UTexture* TempTexture; 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void SetRulesAdditions() override;

	//virtual void OnConstruction(const FTransform& Transform);

private:
	
	void SetupSummer(FLandscapeFragmentRules Rules);
	void SetupSummerToAutumn(FLandscapeFragmentRules Rules);
	void SetupAutumn(FLandscapeFragmentRules Rules);
	void SetupAutumnToWinter(FLandscapeFragmentRules Rules);
	void SetupWinter(FLandscapeFragmentRules Rules);
	void SetupWinterToSpring(FLandscapeFragmentRules Rules);
	void SetupSpring(FLandscapeFragmentRules Rules);
	void SetupSpringToSummer(FLandscapeFragmentRules Rules);


};

