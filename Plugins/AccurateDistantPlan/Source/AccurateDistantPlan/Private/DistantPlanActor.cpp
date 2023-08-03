// Fill out your copyright notice in the Description page of Project Settings.


#include "DistantPlanActor.h"

// Sets default values
ADistantPlanActor::ADistantPlanActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Defaults
	bLowLandscapeBackground = false;
	
	LandscapeScale = 1.0f;

	ELandscapeBackground = EELandscapeBackground::AlpineMountain_A;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	TMap<TEnumAsByte<EELandscapeBackground>, TArray<FLandscapeFragmentRules>> MakeFragmentsRules;
	LandscapeFragmentsRules = MakeFragmentsRules;

	TMap<TEnumAsByte<EELandscapeBackground>, TArray<FLandscapeFragmentRules>> MakeFragmentsRulesLowPoly;
	LandscapeFragmentsRulesLowPoly = MakeFragmentsRulesLowPoly;

	//TArray<ULandscapeFragment*> MakeLandscapeFragments;
	//LandscapeFragments = MakeLandscapeFragments;

	// Construct Landscape Data structures with helpers
	SetupLandscapesRules();

	// Construct low polygon variants Landscape Data structures with helpers
	SetupLandscapesRulesLowPoly();

}


ACultureGameStateBase* ADistantPlanActor::GetGameState() const 
{
	return ::IsValid(GetWorld()) ?
		GetWorld()->GetGameState<ACultureGameStateBase>()
		: nullptr;
}

// Called when the game starts or when spawned
void ADistantPlanActor::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetGameState();

	if (::IsValid(GameState)) {
		GameState->OnSeasonChangeDelegate.AddDynamic(this, &ADistantPlanActor::OnSeasonChange);
	}

	RedrawLandscape(ELandscapeBackground, bLowLandscapeBackground);
	
}

void ADistantPlanActor::OnSeasonChange(TEnumAsByte<EEDemiSeason> Season, float Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("Season: %s"), *UEnum::GetValueAsString(Season));
	UE_LOG(LogTemp, Warning, TEXT("DemiSeason delta: %f"), Delta);

	switch (ELandscapeBackground) {
	case EELandscapeBackground::AfghanMountain_A:
	case EELandscapeBackground::AfghanMountain_B:
	case EELandscapeBackground::Canyon_A:
	case EELandscapeBackground::Canyon_B:
	case EELandscapeBackground::Canyon_C:
	case EELandscapeBackground::DesertDune:
		{
			// That landscapes don't support season changes;
			UE_LOG(LogTemp, Warning, TEXT("OnSeasonChange ignore because of landscape type."));
			return;
		}
	}

	if (CurrentSeason != Season) 
	{
		CurrentSeason = Season;
		ChangeSeason(CurrentSeason);
	}

	switch (Season)
	{
	case EEDemiSeason::WinterToSpring:
	case EEDemiSeason::AutumnToWinter:
	case EEDemiSeason::SpringToSummer:
	case EEDemiSeason::SummerToAutumn:
		SmoothChangeSeason(Delta);
		break;
	}
}

void ADistantPlanActor::ChangeSeason(TEnumAsByte<EEDemiSeason> Season)
{
	UE_LOG(LogTemp, Warning, TEXT("Change Season!"));

	//UE_LOG(LogTemp, Warning, TEXT("LandscapeFragments.Num: %d"), LandscapeFragments.Num());
	TArray<USceneComponent*> SceneChildren;
	Scene->GetChildrenComponents(false, SceneChildren);

	for (USceneComponent* Child : SceneChildren) 
	{
		USeasonLandscapeFragment* Fragment = static_cast<USeasonLandscapeFragment*>(Child);
		if (::IsValid(Fragment))
		{
			Fragment->ChangeSeason(CurrentSeason);
		}
	}
}

//
void ADistantPlanActor::SmoothChangeSeason(float Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("Smooth Change Season!"));
	UE_LOG(LogTemp, Warning, TEXT("Smooth Change Season delta: %f"), Delta);

	//UE_LOG(LogTemp, Warning, TEXT("LandscapeFragments.Num: %d"), LandscapeFragments.Num());
	TArray<USceneComponent*> SceneChildren;
	Scene->GetChildrenComponents(false, SceneChildren);

	for (USceneComponent* Child : SceneChildren)
	{
		USeasonLandscapeFragment* Fragment = static_cast<USeasonLandscapeFragment*>(Child);
		if (::IsValid(Fragment))
		{
			Fragment->SmoothChangeSeason(Delta);
		}
	}
}

//
void ADistantPlanActor::OnConstruction(const FTransform& Transform)
{
	RedrawLandscape(ELandscapeBackground, bLowLandscapeBackground);
}

//
void ADistantPlanActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropName = PropertyChangedEvent.GetPropertyName();

	if (PropName == GET_MEMBER_NAME_CHECKED(ADistantPlanActor, ELandscapeBackground))
	{
		RedrawLandscape(ELandscapeBackground, bLowLandscapeBackground);
	}

	if (PropName == GET_MEMBER_NAME_CHECKED(ADistantPlanActor, bLowLandscapeBackground))
	{
		RedrawLandscape(ELandscapeBackground, bLowLandscapeBackground);
	}

	if (PropName == GET_MEMBER_NAME_CHECKED(ADistantPlanActor, LandscapeScale))
	{
		RedrawLandscape(ELandscapeBackground, bLowLandscapeBackground);
	}
}

void ADistantPlanActor::RedrawLandscape(TEnumAsByte<EELandscapeBackground> CurrentLandscape, bool LowMode)
{
	UE_LOG(LogTemp, Warning, TEXT("LandscapeBackground: %s"), *UEnum::GetValueAsString(CurrentLandscape));
	UE_LOG(LogTemp, Warning, TEXT("LowMode: %s"), LowMode ? TEXT("True") : TEXT("False"));

	/* Destroy Scene Children */
	ClearScene();

	TArray<FLandscapeFragmentRules>* RulesPtr;
	
	if (LowMode) 
	{
		RulesPtr = LandscapeFragmentsRulesLowPoly.Find(CurrentLandscape);
	}
	else {
		RulesPtr = LandscapeFragmentsRules.Find(CurrentLandscape);
	}
	
	
	if (RulesPtr != nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Landscape Fragment Rules for this background"));
	}

	switch (CurrentLandscape) {
	case EELandscapeBackground::AlpineMountain_A: 
		{
			if (RulesPtr != nullptr)
			{
				MakeSeasonFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_B:
		{
			if (RulesPtr != nullptr)
			{
				MakeSeasonFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_C:
		{
			if (RulesPtr != nullptr)
			{
				MakeSeasonFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_D:
		{
			if (RulesPtr != nullptr)
			{
				MakeSeasonFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_E:
		{
			if (RulesPtr != nullptr)
			{
				MakeSeasonFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AfghanMountain_A:
		{
			if (RulesPtr != nullptr)
			{
				MakeFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AfghanMountain_B:
		{
			if (RulesPtr != nullptr)
			{
				MakeFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::Canyon_A:
		{
			if (RulesPtr != nullptr)
			{
				MakeFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::Canyon_B:
		{
			if (RulesPtr != nullptr)
			{
				MakeFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::Canyon_C:
		{
			if (RulesPtr != nullptr)
			{
				MakeFragments(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::DesertDune:
		{
			if (RulesPtr != nullptr)
			{
				MakeFragments(*RulesPtr);
			}
			break;
		}
	}
	
}

void ADistantPlanActor::ClearScene() {

	/* Destroy Scene Children */
	TArray<USceneComponent*> SceneChildren;
	Scene->GetChildrenComponents(true, SceneChildren);

	for (USceneComponent* Child : SceneChildren) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroing static mesh fragment..."));
		Child->DestroyComponent();
	}
}

void ADistantPlanActor::SetupFragment(ULandscapeFragment* LandscapeFragment)
{
	LandscapeFragment->CreationMethod = EComponentCreationMethod::Native;
	LandscapeFragment->SetMobility(EComponentMobility::Movable);

	LandscapeFragment->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
	LandscapeFragment->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LandscapeFragment->SetWorldScale3D(FVector(1.0f));
}

void ADistantPlanActor::MakeFragments(const TArray<FLandscapeFragmentRules>& RulesList)
{
	for (const FLandscapeFragmentRules& Rules : RulesList) {

		ULandscapeFragment* Fragment = NewObject<ULandscapeFragment>(Scene);
		SetupFragment(Fragment);

		Fragment->SetLandscapeScale(LandscapeScale);

		if (::IsValid(Fragment))
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Fragment rules for Fragment."));
			Fragment->SetRules(Rules);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Rules not used."));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("LandscapeFragments.Num: %d"), LandscapeFragments.Num());
}

void ADistantPlanActor::MakeSeasonFragments(const TArray<FLandscapeFragmentRules>& RulesList)
{
	for (const FLandscapeFragmentRules& Rules : RulesList) 
	{
		USeasonLandscapeFragment* Fragment = NewObject<USeasonLandscapeFragment>(Scene);
		SetupFragment(Fragment);

		Fragment->SetLandscapeScale(LandscapeScale);

		if (::IsValid(Fragment))
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Fragment rules for Season Fragment."));
			Fragment->SetRules(Rules);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Rules not used."));
		}
	}

}

//
void  ADistantPlanActor::SetupLandscapesRules()
{
	
	FString MOUNTAIN_PATH = "/Game/PhotoR_Backgrounds/Meshes/360d_Horizon";
	FString MOUNTAIN_TEXTURE_PATH = "/Game/PhotoR_Backgrounds/Textures";

	/* Alpine Mountain Variants */
	FString AlpineMountainId_A{ "A" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsA = MakeAlpineMountain(MOUNTAIN_PATH, AlpineMountainId_A);
	BindMountainTextures(AlpineFragmentsA, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_A);
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_A, AlpineFragmentsA);

	FString AlpineMountainId_B{ "B" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsB = MakeAlpineMountain(MOUNTAIN_PATH, AlpineMountainId_B);
	BindMountainTextures(AlpineFragmentsB, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_B);
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_B, AlpineFragmentsB);

	FString AlpineMountainId_C{ "C" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsC = MakeAlpineMountain(MOUNTAIN_PATH, AlpineMountainId_C);
	BindMountainTextures(AlpineFragmentsC, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_C);
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_C, AlpineFragmentsC);

	FString AlpineMountainId_D{ "D" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsD = MakeAlpineMountain(MOUNTAIN_PATH, AlpineMountainId_D);
	BindMountainTextures(AlpineFragmentsD, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_D);
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_D, AlpineFragmentsD);

	FString AlpineMountainId_E{ "E" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsE = MakeAlpineMountain(MOUNTAIN_PATH, AlpineMountainId_E);
	BindMountainTextures(AlpineFragmentsD, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_E);
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_E, AlpineFragmentsE);
	
	/* Afghan Variants */

	TArray<FLandscapeFragmentRules> AfghanFragmentsA = MakeAfghanMountain(MOUNTAIN_PATH, FString("A"));
	LandscapeFragmentsRules.Add(EELandscapeBackground::AfghanMountain_A, AfghanFragmentsA);

	TArray<FLandscapeFragmentRules> AfghanFragmentsB = MakeAfghanMountain(MOUNTAIN_PATH, FString("B"));
	LandscapeFragmentsRules.Add(EELandscapeBackground::AfghanMountain_B, AfghanFragmentsB);

	/* Canyon Variants */

	TArray<FLandscapeFragmentRules> CanyonFragmentsA = MakeCanyon(MOUNTAIN_PATH, FString("A"));
	LandscapeFragmentsRules.Add(EELandscapeBackground::Canyon_A, CanyonFragmentsA);

	TArray<FLandscapeFragmentRules> CanyonFragmentsB = MakeCanyon(MOUNTAIN_PATH, FString("B"));
	LandscapeFragmentsRules.Add(EELandscapeBackground::Canyon_B, CanyonFragmentsB);

	TArray<FLandscapeFragmentRules> CanyonFragmentsC = MakeCanyon(MOUNTAIN_PATH, FString("C"));
	LandscapeFragmentsRules.Add(EELandscapeBackground::Canyon_C, CanyonFragmentsC);

	/* Desert */

	TArray<FLandscapeFragmentRules> DesertFragments = MakeDesert(MOUNTAIN_PATH);
	LandscapeFragmentsRules.Add(EELandscapeBackground::DesertDune, DesertFragments);

}

void ADistantPlanActor::SetupLandscapesRulesLowPoly()
{
	FString MOUNTAIN_PATH = "/Game/PhotoR_Backgrounds/Meshes/360d_Horizon";
	FString MOUNTAIN_TEXTURE_PATH = "/Game/PhotoR_Backgrounds/Textures";

	/* Alpine Mountain Variants */

	FString AlpineMountainId_A{ "A" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsA = MakeAlpineMountainLowPoly(MOUNTAIN_PATH, AlpineMountainId_A);
	BindMountainTextures(AlpineFragmentsA, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_A);
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::AlpineMountain_A, AlpineFragmentsA);

	FString AlpineMountainId_B{ "B" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsB = MakeAlpineMountainLowPoly(MOUNTAIN_PATH, AlpineMountainId_B);
	BindMountainTextures(AlpineFragmentsB, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_B);
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::AlpineMountain_B, AlpineFragmentsB);

	FString AlpineMountainId_C{ "C" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsC = MakeAlpineMountainLowPoly(MOUNTAIN_PATH, AlpineMountainId_C);
	BindMountainTextures(AlpineFragmentsC, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_C);
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::AlpineMountain_C, AlpineFragmentsC);

	FString AlpineMountainId_D{ "D" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsD = MakeAlpineMountainLowPoly(MOUNTAIN_PATH, AlpineMountainId_D);
	BindMountainTextures(AlpineFragmentsD, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_D);
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::AlpineMountain_D, AlpineFragmentsD);

	FString AlpineMountainId_E{ "E" };
	TArray<FLandscapeFragmentRules> AlpineFragmentsE = MakeAlpineMountainLowPoly(MOUNTAIN_PATH, AlpineMountainId_E);
	BindMountainTextures(AlpineFragmentsE, MOUNTAIN_TEXTURE_PATH, AlpineMountainId_E);
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::AlpineMountain_E, AlpineFragmentsE);

	/* Afghan Variants */

	TArray<FLandscapeFragmentRules> AfghanFragmentsA = MakeAfghanMountainLowPoly(MOUNTAIN_PATH, FString("A"));
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::AfghanMountain_A, AfghanFragmentsA);

	TArray<FLandscapeFragmentRules> AfghanFragmentsB = MakeAfghanMountainLowPoly(MOUNTAIN_PATH, FString("B"));
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::AfghanMountain_B, AfghanFragmentsB);

	/* Canyon Variants */

	TArray<FLandscapeFragmentRules> CanyonFragmentsA = MakeCanyonLowPoly(MOUNTAIN_PATH, FString("A"));
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::Canyon_A, CanyonFragmentsA);

	TArray<FLandscapeFragmentRules> CanyonFragmentsB = MakeCanyonLowPoly(MOUNTAIN_PATH, FString("B"));
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::Canyon_B, CanyonFragmentsB);

	TArray<FLandscapeFragmentRules> CanyonFragmentsC = MakeCanyonLowPoly(MOUNTAIN_PATH, FString("C"));
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::Canyon_C, CanyonFragmentsC);

	/* Desert */

	TArray<FLandscapeFragmentRules> DesertFragments = MakeDesertLowPoly(MOUNTAIN_PATH);
	LandscapeFragmentsRulesLowPoly.Add(EELandscapeBackground::DesertDune, DesertFragments);
}

/* Bind Alpine Mountain Textures for Accurate Seasons */
void  ADistantPlanActor::BindMountainTextures(
	TArray<FLandscapeFragmentRules>& FragmentRules, FString Path, FString Id)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(Id.ToLower()));

	// Spring Textures
	FString Spring_BC_Red_1_Source = FString::Format(TEXT("{0}/grass_spring_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_BC_Red_1_Found(*Spring_BC_Red_1_Source);

	FString Spring_BC_Red_2_Source = FString::Format(TEXT("{0}/grass_spring_b_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_BC_Red_2_Found(*Spring_BC_Red_2_Source);

	FString Spring_BC_Green_Source = FString::Format(TEXT("{0}/SM_ground_gravel_b_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_BC_Green_Found(*Spring_BC_Green_Source);

	FString Spring_BC_Blue_Source = FString::Format(TEXT("{0}/SM_ground_gravel_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_BC_Blue_Found(*Spring_BC_Blue_Source);

	// Spring snow
	FString Spring_BC_Snow_Source = FString::Format(TEXT("{0}/SM_snow_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_BC_Snow_Found(*Spring_BC_Snow_Source);

	// Summer Textures 
	FString Summer_BC_Red_1_Source = FString::Format(TEXT("{0}/SM_grass_c_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_BC_Red_1_Found(*Summer_BC_Red_1_Source);

	FString Summer_BC_Red_2_Source = FString::Format(TEXT("{0}/grass_fall_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_BC_Red_2_Found(*Summer_BC_Red_2_Source);

	FString Summer_BC_Green_Source = FString::Format(TEXT("{0}/SM_sand_pebbles_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_BC_Green_Found(*Summer_BC_Green_Source);

	FString Summer_BC_Blue_Source = FString::Format(TEXT("{0}/SM_ground_gravel_b_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_BC_Blue_Found(*Summer_BC_Blue_Source);

	// Autumn Textures 
	FString Autumn_BC_Red_1_Source = FString::Format(TEXT("{0}/grass_summer_b_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_BC_Red_1_Found(*Autumn_BC_Red_1_Source);

	FString Autumn_BC_Red_2_Source = FString::Format(TEXT("{0}/grass_summer_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_BC_Red_2_Found(*Autumn_BC_Red_2_Source);

	FString Autumn_BC_Green_Source = FString::Format(TEXT("{0}/SM_sand_pebbles_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_BC_Green_Found(*Autumn_BC_Green_Source);

	FString Autumn_BC_Blue_Source = FString::Format(TEXT("{0}/SM_ground_gravel_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_BC_Blue_Found(*Autumn_BC_Blue_Source);

	// Winter Texture 
	// Only one texture used for all channels
	FString Winter_BC_Source = FString::Format(TEXT("{0}/SM_rock_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Winter_BC_Found(*Winter_BC_Source);

	// Winter snow
	FString Winter_Snow_Source = FString::Format(TEXT("{0}/SM_snow_a_d"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Winter_Snow_Found(*Winter_Snow_Source);

	// Normals
	  
	// Spring Normals
	FString Spring_Normal_Red_1_Source = FString::Format(TEXT("{0}/SM_grass_c_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_Normal_Red_1_Found(*Spring_Normal_Red_1_Source);

	FString Spring_Normal_Red_2_Source = FString::Format(TEXT("{0}/grass_fall_b_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_Normal_Red_2_Found(*Spring_Normal_Red_2_Source);

	FString Spring_Normal_Green_Source = FString::Format(TEXT("{0}/SM_ground_gravel_b_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_Normal_Green_Found(*Spring_Normal_Green_Source);

	FString Spring_Normal_Blue_Source = FString::Format(TEXT("{0}/SM_ground_gravel_c_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_Normal_Blue_Found(*Spring_Normal_Blue_Source);

	// Snow normal
	FString Spring_Normal_Snow_Source = FString::Format(TEXT("{0}/SM_snow_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Spring_Normal_Snow_Found(*Spring_Normal_Snow_Source);

	// Summer Normals 

	FString Summer_Normal_Red_1_Source = FString::Format(TEXT("{0}/grass_fall_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_Normal_Red_1_Found(*Summer_Normal_Red_1_Source);

	FString Summer_Normal_Red_2_Source = FString::Format(TEXT("{0}/SM_grass_c_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_Normal_Red_2_Found(*Summer_Normal_Red_2_Source);

	FString Summer_Normal_Green_Source = FString::Format(TEXT("{0}/SM_sand_pebbles_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_Normal_Green_Found(*Summer_Normal_Green_Source);

	FString Summer_Normal_Blue_Source = FString::Format(TEXT("{0}/SM_ground_gravel_b_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Summer_Normal_Blue_Found(*Summer_Normal_Blue_Source);

	// Autumn normals
	FString Autumn_Normal_Red_1_Source = FString::Format(TEXT("{0}/grass_summer_b_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_Normal_Red_1_Found(*Autumn_Normal_Red_1_Source);

	FString Autumn_Normal_Red_2_Source = FString::Format(TEXT("{0}/grass_summer_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_Normal_Red_2_Found(*Autumn_Normal_Red_2_Source);

	FString Autumn_Normal_Green_Source = FString::Format(TEXT("{0}/SM_sand_pebbles_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_Normal_Green_Found(*Autumn_Normal_Green_Source);

	FString Autumn_Normal_Blue_Source = FString::Format(TEXT("{0}/SM_ground_gravel_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Autumn_Normal_Blue_Found(*Autumn_Normal_Blue_Source);

	// Winter normal 
	FString Winter_BC_Normal_Source = FString::Format(TEXT("{0}/SM_rock_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Winter_BC_Normal_Found(*Winter_BC_Normal_Source);

	// Winter snow normal
	FString Winter_Snow_Normal_Source = FString::Format(TEXT("{0}/SM_snow_a_n"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Winter_Snow_Normal_Found(*Winter_Snow_Normal_Source);

	// Main Normals
	FString Main_Normal_Source = FString::Format(TEXT("{0}/pr_background_mountain_{1}_N"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Main_Normal_Found(*Main_Normal_Source);

	// Material splats
	FString Splat_Source = FString::Format(TEXT("{0}/pr_background_mountain_{1}_SPLAT"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Splat_Found(*Splat_Source);

	FString Winter_Splat_Source = FString::Format(TEXT("{0}/pr_background_mountain_{1}_winter_SPLAT"), Args);
	ConstructorHelpers::FObjectFinder<UTexture2D> Winter_Splat_Found(*Winter_Splat_Source);
	
	
	for (FLandscapeFragmentRules& Rules : FragmentRules)
	{
		/* Spring textures setup */
		if (Spring_BC_Red_1_Found.Succeeded())
		{
			Rules.Spring_BC_Red_1_T = Spring_BC_Red_1_Found.Object;
		}

		if (Spring_BC_Red_2_Found.Succeeded())
		{
			Rules.Spring_BC_Red_2_T = Spring_BC_Red_2_Found.Object;
		}

		if (Spring_BC_Green_Found.Succeeded())
		{
			Rules.Spring_BC_Green_T = Spring_BC_Green_Found.Object;
		}

		if (Spring_BC_Blue_Found.Succeeded())
		{
			Rules.Spring_BC_Blue_T = Spring_BC_Blue_Found.Object;
		}

		if (Spring_BC_Snow_Found.Succeeded())
		{
			Rules.Spring_BC_Snow_T = Spring_BC_Snow_Found.Object;
		}

		/* Summer textures setup */
		if (Summer_BC_Red_1_Found.Succeeded())
		{
			Rules.Summer_BC_Red_1_T = Summer_BC_Red_1_Found.Object;
		}

		if (Summer_BC_Red_2_Found.Succeeded())
		{
			Rules.Summer_BC_Red_2_T = Summer_BC_Red_2_Found.Object;
		}

		if (Summer_BC_Green_Found.Succeeded())
		{
			Rules.Summer_BC_Green_T = Summer_BC_Green_Found.Object;
		}

		if (Summer_BC_Blue_Found.Succeeded())
		{
			Rules.Summer_BC_Blue_T = Summer_BC_Blue_Found.Object;
		}

		/* Autumn textures setup */
		if (Autumn_BC_Red_1_Found.Succeeded())
		{
			Rules.Autumn_BC_Red_1_T = Autumn_BC_Red_1_Found.Object;
		}

		if (Autumn_BC_Red_2_Found.Succeeded())
		{
			Rules.Autumn_BC_Red_2_T = Autumn_BC_Red_2_Found.Object;
		}

		if (Autumn_BC_Green_Found.Succeeded())
		{
			Rules.Autumn_BC_Green_T = Autumn_BC_Green_Found.Object;
		}

		if (Autumn_BC_Blue_Found.Succeeded())
		{
			Rules.Autumn_BC_Blue_T = Autumn_BC_Blue_Found.Object;
		}

		/* Winter textures setup */
		if (Winter_BC_Found.Succeeded())
		{
			Rules.Winter_BC_T = Winter_BC_Found.Object;
		}

		if (Winter_Snow_Found.Succeeded())
		{
			Rules.Winter_Snow_T = Winter_Snow_Found.Object;
		}

		// Normals
		
		// Spring normals 
		if (Spring_Normal_Red_1_Found.Succeeded())
		{
			Rules.Spring_Normal_Red_1_T = Spring_BC_Red_1_Found.Object;
		}

		if (Spring_Normal_Red_2_Found.Succeeded())
		{
			Rules.Spring_Normal_Red_2_T = Spring_BC_Red_2_Found.Object;
		}

		if (Spring_Normal_Green_Found.Succeeded())
		{
			Rules.Spring_Normal_Green_T = Spring_BC_Green_Found.Object;
		}

		if (Spring_Normal_Blue_Found.Succeeded())
		{
			Rules.Spring_Normal_Blue_T = Spring_BC_Blue_Found.Object;
		}

		// Snow normal
		if (Spring_Normal_Snow_Found.Succeeded())
		{
			Rules.Spring_Normal_Snow_T = Spring_Normal_Snow_Found.Object;
		}

		/* Spring textures setup */
		if (Summer_Normal_Red_1_Found.Succeeded())
		{
			Rules.Summer_Normal_Red_1_T = Summer_BC_Red_2_Found.Object;
		}

		if (Summer_Normal_Red_2_Found.Succeeded())
		{
			Rules.Summer_Normal_Red_2_T = Summer_BC_Red_2_Found.Object;
		}

		if (Summer_Normal_Green_Found.Succeeded())
		{
			Rules.Summer_Normal_Green_T = Summer_Normal_Green_Found.Object;
		}

		if (Summer_Normal_Blue_Found.Succeeded())
		{
			Rules.Summer_Normal_Blue_T = Summer_Normal_Blue_Found.Object;
		}

		/* Autumn normal */

		if (Autumn_Normal_Red_1_Found.Succeeded())
		{
			Rules.Autumn_Normal_Red_1_T = Autumn_BC_Red_1_Found.Object;
		}

		if (Autumn_Normal_Red_2_Found.Succeeded())
		{
			Rules.Autumn_Normal_Red_2_T = Autumn_BC_Red_2_Found.Object;
		}

		if (Autumn_Normal_Green_Found.Succeeded())
		{
			Rules.Autumn_Normal_Green_T = Autumn_Normal_Green_Found.Object;
		}

		if (Autumn_Normal_Blue_Found.Succeeded())
		{
			Rules.Autumn_Normal_Blue_T = Autumn_Normal_Blue_Found.Object;
		}

		// Winter normal
		if (Winter_BC_Normal_Found.Succeeded())
		{
			Rules.Winter_BC_Normal_T = Winter_BC_Normal_Found.Object;
		}

		if (Winter_Snow_Normal_Found.Succeeded())
		{
			Rules.Winter_Snow_Normal_T = Winter_Snow_Normal_Found.Object;
		}


		// Main Normals
		if (Main_Normal_Found.Succeeded())
		{
			Rules.Main_Normal_T = Main_Normal_Found.Object;
		}

		if (Splat_Found.Succeeded())
		{
			Rules.Splat_T = Splat_Found.Object;
		}

		if (Winter_Splat_Found.Succeeded())
		{
			Rules.Winter_Splat_T = Winter_Splat_Found.Object;
		}
	}
}

/* Landscape Fragment Rules Makers*/

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeAlpineMountain(
	FString Path, FString Id)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(Id.ToLower()));

	FString FullPath = FString::Format(TEXT("{0}/Mountain_{1}"), Args);
	TArray<FString> Urls;
	Urls.Add(FString::Printf(TEXT("PR_background_%s_E1"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_E2"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_N1"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_N2"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_S1"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_S2"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_W1"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_W2"), *Id));

	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeAfghanMountain(
	FString Path, FString Id)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(Id.ToLower()));

	FString FullPath = FString::Format(TEXT("{0}/Afghan_Mountain_{1}"), Args);
	TArray<FString> Urls;
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_D"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_L"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_U"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_R"), *Id));
	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeCanyon(
	FString Path, FString Id)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(Id.ToLower()));

	FString FullPath = FString::Format(TEXT("{0}/Canyon_{1}"), Args);
	TArray<FString> Urls;
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_D"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_L"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_U"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_R"), *Id));
	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeDesert(FString Path)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));

	FString FullPath = FString::Format(TEXT("{0}/Desert"), Args);
	TArray<FString> Urls;
	Urls.Add(FString("PR_desert_D"));
	Urls.Add(FString("PR_desert_L"));
	Urls.Add(FString("PR_desert_U"));
	Urls.Add(FString("PR_desert_R"));
	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}

/* LowPoly Landcape Fragment Rules Makers */

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeAlpineMountainLowPoly(
	FString Path, FString Id)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(Id.ToLower()));

	FString FullPath = FString::Format(TEXT("{0}/Mountain_{1}_low"), Args);
	TArray<FString> Urls;
	Urls.Add(FString::Printf(TEXT("PR_background_%s_E1_LOW"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_E2_LOW"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_N1_LOW"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_N2_LOW"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_S1_LOW"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_S2_LOW"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_W1_LOW"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_background_%s_W2_LOW"), *Id));

	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeAfghanMountainLowPoly(
	FString Path, FString Id)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(Id.ToLower()));

	FString FullPath = FString::Format(TEXT("{0}/Afghan_Mountain_{1}_low"), Args);
	TArray<FString> Urls;
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_low_D"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_low_L"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_low_U"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_afgan_mountain_%s_low_R"), *Id));
	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeCanyonLowPoly(
	FString Path, FString Id)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(Id.ToLower()));

	FString FullPath = FString::Format(TEXT("{0}/Canyon_{1}_low"), Args);
	TArray<FString> Urls;
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_low_D"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_low_L"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_low_U"), *Id));
	Urls.Add(FString::Printf(TEXT("PR_canyon_%s_low_R"), *Id));
	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeDesertLowPoly(FString Path)
{
	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));

	FString FullPath = FString::Format(TEXT("{0}/Desert_low"), Args);
	TArray<FString> Urls;
	Urls.Add(FString("PR_desert_low_D"));
	Urls.Add(FString("PR_desert_low_L"));
	Urls.Add(FString("PR_desert_low_U"));
	Urls.Add(FString("PR_desert_low_R"));
	TArray<FLandscapeFragmentRules> Fragments =
		MakeLandscapeFragments(FullPath, Urls);

	return Fragments;
}


/* Signle Fragment maker */

TArray<FLandscapeFragmentRules> ADistantPlanActor::MakeLandscapeFragments(
	FString Path, const TArray<FString>& FragmentUrls)
{
	TArray<FLandscapeFragmentRules> Fragments;

	for (FString Url : FragmentUrls) 
	{
		TArray< FStringFormatArg > args;
		args.Add(FStringFormatArg(Path));
		args.Add(FStringFormatArg(Url));

		FString FullSource = FString::Format(TEXT("StaticMesh'{0}/{1}'"), args);

		ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(*FullSource);

		if (Mesh.Succeeded())
		{
			FLandscapeFragmentRules Fragment;
			Fragment.StaticMesh = Mesh.Object;
			Fragments.Add(Fragment);

			UE_LOG(LogTemp, Warning, TEXT("%s.Succeeded"), *FullSource);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT(" %s.Not succeeded."), *FullSource);
		}
	}

	return Fragments;
}

