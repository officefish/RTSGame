// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CultureGameStateBase.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"

#include "Engine/Texture2D.h"

#include "LandscapeFragment.h"
#include "SeasonLandscapeFragment.h"

#include "DistantPlanActor.generated.h"


UENUM(BlueprintType)
enum class EELandscapeBackground : uint8
{
	AlpineMountain_A			UMETA(DisplayName = "AlpineMountain_A"),
	AlpineMountain_B			UMETA(DisplayName = "AlpineMountain_B"),
	AlpineMountain_C			UMETA(DisplayName = "AlpineMountain_C"),
	AlpineMountain_D			UMETA(DisplayName = "AlpineMountain_D"),
	AlpineMountain_E			UMETA(DisplayName = "AlpineMountain_E"),

	AfghanMountain_A			UMETA(DisplayName = "AfghanMountain_A"),
	AfghanMountain_B			UMETA(DisplayName = "AfghanMountain_B"),

	Canyon_A					UMETA(DisplayName = "Canyon_A"),
	Canyon_B					UMETA(DisplayName = "Canyon_B"),
	Canyon_C					UMETA(DisplayName = "Canyon_C"),

	DesertDune					UMETA(DisplayName = "DesertDune"),

};


UCLASS()
class ACCURATEDISTANTPLAN_API ADistantPlanActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADistantPlanActor();

public:
	/* UFunctions */

	UFUNCTION()
		void OnSeasonChange(TEnumAsByte<EEDemiSeason> Season, float Delta);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		TEnumAsByte<EELandscapeBackground> ELandscapeBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		bool bLowLandscapeBackground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan", meta = (ClampMin = "1.0", ClampMax = "4.0", UIMin = "1.0", UIMax = "4.0"));
		float LandscapeScale;

	/* Components*/

	UPROPERTY(BlueprintReadWrite, NonTransactional, meta = (Category = "AccurateDistantPlan", OverrideNativeName = "Scene"))
		USceneComponent* Scene;


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Editor Settings
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called onConstruction Event
	virtual void OnConstruction(const FTransform& Transform);

protected:

	void ChangeSeason(TEnumAsByte<EEDemiSeason> Season);
	void SmoothChangeSeason(float Delta);

	void SetupFragment(ULandscapeFragment* LandscapeFragment);
	void MakeFragments(const TArray<FLandscapeFragmentRules>& RulesList);
	void MakeSeasonFragments(const TArray<FLandscapeFragmentRules>& RulesList);
	
	void SetupLandscapesRules();
	void SetupLandscapesRulesLowPoly();
	TArray<FLandscapeFragmentRules> MakeAlpineMountain(FString Path, FString Id);
	TArray<FLandscapeFragmentRules> MakeAfghanMountain(FString Path, FString Id);
	TArray<FLandscapeFragmentRules> MakeCanyon(FString Path, FString Id);
	TArray<FLandscapeFragmentRules> MakeDesert(FString Path);

	TArray<FLandscapeFragmentRules> MakeAlpineMountainLowPoly(FString Path, FString Id);
	TArray<FLandscapeFragmentRules> MakeAfghanMountainLowPoly(FString Path, FString Id);
	TArray<FLandscapeFragmentRules> MakeCanyonLowPoly(FString Path, FString Id);
	TArray<FLandscapeFragmentRules> MakeDesertLowPoly(FString Path);

	void BindMountainTextures(TArray<FLandscapeFragmentRules>& FragmentRules, FString Path, FString Id);

	TArray<FLandscapeFragmentRules> MakeLandscapeFragments(FString Path, const TArray<FString>& FragmentUrls);
	void ClearScene();

private:

	void RedrawLandscape(TEnumAsByte<EELandscapeBackground> CurrentLandscape, bool LowMode);

	TMap<TEnumAsByte<EELandscapeBackground>, TArray<FLandscapeFragmentRules>> LandscapeFragmentsRules;
	TMap<TEnumAsByte<EELandscapeBackground>, TArray<FLandscapeFragmentRules>> LandscapeFragmentsRulesLowPoly;

	TArray<ULandscapeFragment*> LandscapeFragments;

	ACultureGameStateBase* GameState;
	ACultureGameStateBase* GetGameState() const;

	TEnumAsByte<EEDemiSeason> CurrentSeason;

};
