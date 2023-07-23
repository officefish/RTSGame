// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AccurateDayNightStateBase.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "DistantPlanActor.generated.h"


UENUM(BlueprintType)
enum class EELandscapeBackground : uint8
{
	AlpineMountain_A			UMETA(DisplayName = "AlpineMountain_A"),
	AlpineMountain_B			UMETA(DisplayName = "AlpineMountain_B"),
	AlpineMountain_C			UMETA(DisplayName = "AlpineMountain_C"),
	AlpineMountain_D			UMETA(DisplayName = "AlpineMountain_D"),
	AlpineMountain_E			UMETA(DisplayName = "AlpineMountain_E"),

	AfganMountain_A				UMETA(DisplayName = "AfganMountain_A"),
	AfganMountain_B				UMETA(DisplayName = "AfganMountain_B"),

	Canyon_A					UMETA(DisplayName = "Canyon_A"),
	Canyon_B					UMETA(DisplayName = "Canyon_B"),
	Canyon_C					UMETA(DisplayName = "Canyon_C"),

	DesertDune					UMETA(DisplayName = "DesertDune"),

};

USTRUCT(BlueprintType)
struct FLandscapeFragmentRules
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		FVector Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccurateDistantPlan")
		UStaticMesh* StaticMesh;


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
	virtual void Tick(float DeltaTime) override;

	// Called onConstruction Event
	virtual void OnConstruction(const FTransform& Transform);

protected:

	void SetupFragment(UStaticMeshComponent* LandscapeFragment);
	void MakeFragments(int8 NumFragments);
	void SetFragmentsRules(const TArray<FLandscapeFragmentRules>& Rules);

	void SetupLandscapesRulesWithHelpers();
	TArray<FLandscapeFragmentRules> MakeAlpineMountain(FString Path, FString Id);
	TArray<FLandscapeFragmentRules> MakeLandscapeFragments(FString Path, const TArray<FString>& FragmentUrls);
	void ClearScene();

private:

	void RedrawLandscape(TEnumAsByte<EELandscapeBackground> CurrentLandscape, bool LowMode);

	TMap<TEnumAsByte<EELandscapeBackground>, TArray<FLandscapeFragmentRules>> LandscapeFragmentsRules;

	AAccurateDayNightStateBase* GameState;
	AAccurateDayNightStateBase* GetGameState() const;

};
