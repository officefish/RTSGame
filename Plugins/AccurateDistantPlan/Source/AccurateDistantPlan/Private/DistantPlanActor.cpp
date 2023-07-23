// Fill out your copyright notice in the Description page of Project Settings.


#include "DistantPlanActor.h"

// Sets default values
ADistantPlanActor::ADistantPlanActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Defaults
	bLowLandscapeBackground = true;
	ELandscapeBackground = EELandscapeBackground::AlpineMountain_A;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	TMap<TEnumAsByte<EELandscapeBackground>, TArray<FLandscapeFragmentRules>> MakeFragmentsRules;
	LandscapeFragmentsRules = MakeFragmentsRules;

	// Construct Landscape Data structures with helpers
	SetupLandscapesRulesWithHelpers();

}




AAccurateDayNightStateBase* ADistantPlanActor::GetGameState() const {
	return ::IsValid(GetWorld()) ?
		GetWorld()->GetGameState<AAccurateDayNightStateBase>()
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
	
}

void ADistantPlanActor::OnSeasonChange(TEnumAsByte<EEDemiSeason> Season, float Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("Season: %s"), *UEnum::GetValueAsString(Season));
	UE_LOG(LogTemp, Warning, TEXT("DemiSeason delta: %f"), Delta);
}


// Called every frame
void ADistantPlanActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADistantPlanActor::OnConstruction(const FTransform& Transform)
{
	RedrawLandscape(ELandscapeBackground, bLowLandscapeBackground);
}



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
}

void ADistantPlanActor::RedrawLandscape(TEnumAsByte<EELandscapeBackground> CurrentLandscape, bool LowMode)
{
	UE_LOG(LogTemp, Warning, TEXT("LandscapeBackground: %s"), *UEnum::GetValueAsString(CurrentLandscape));
	UE_LOG(LogTemp, Warning, TEXT("LowMode: %s"), LowMode ? TEXT("True") : TEXT("False"));

	TArray<FLandscapeFragmentRules>* RulesPtr = LandscapeFragmentsRules.Find(CurrentLandscape);
	if (RulesPtr != nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Landscape Fragment Rules for this background"));
	}

	switch (CurrentLandscape) {
	case EELandscapeBackground::AlpineMountain_A: 
		{
			MakeFragments(8);
			if (RulesPtr != nullptr)
			{
				SetFragmentsRules(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_B:
		{
			MakeFragments(8);
			if (RulesPtr != nullptr)
			{
				SetFragmentsRules(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_C:
		{
			MakeFragments(8);
			if (RulesPtr != nullptr)
			{
				SetFragmentsRules(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_D:
		{
			MakeFragments(8);
			if (RulesPtr != nullptr)
			{
				SetFragmentsRules(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AlpineMountain_E:
		{
			MakeFragments(8);
			if (RulesPtr != nullptr)
			{
				SetFragmentsRules(*RulesPtr);
			}
			break;
		}
	case EELandscapeBackground::AfganMountain_A:
		{
			MakeFragments(4);
			break;
		}
	case EELandscapeBackground::AfganMountain_B:
		{
			MakeFragments(4);
			break;
		}
	case EELandscapeBackground::Canyon_A:
		{
			MakeFragments(4);
			break;
		}
	case EELandscapeBackground::Canyon_B:
		{
			MakeFragments(4);
			break;
		}
	case EELandscapeBackground::Canyon_C:
		{
			MakeFragments(4);
			break;
		}
	case EELandscapeBackground::DesertDune:
		{
			MakeFragments(4);
			break;
		}
	}
	
}

void ADistantPlanActor::ClearScene() {

	/* Destroy Scene Children */

	TArray<USceneComponent*> SceneChildren;
	Scene->GetChildrenComponents(true, SceneChildren);

	for (USceneComponent* Child : SceneChildren) {

		UStaticMeshComponent* Fragment = static_cast<UStaticMeshComponent*>(Child);
		if (::IsValid(Fragment))
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroing static mesh fragment..."));
			Fragment->DestroyComponent();
		}
	}
}

void ADistantPlanActor::SetupFragment(UStaticMeshComponent* LandscapeFragment)
{
	LandscapeFragment->CreationMethod = EComponentCreationMethod::Native;
	LandscapeFragment->SetMobility(EComponentMobility::Movable);
	//LandscapeFragment->OverrideMaterials = TArray<UMaterialInterface*>();
	//LandscapeFragment->OverrideMaterials.Add(nullptr);

	LandscapeFragment->SetCollisionProfileName(FName(TEXT("NoCollision")));
	//LandscapeFragment->SetRelativeScale3D(FVector(400.000000, 400.000000, 400.000000));
	LandscapeFragment->bVisibleInRayTracing = false;
	LandscapeFragment->bReceivesDecals = false;
	LandscapeFragment->bAffectDynamicIndirectLighting = false;

	/* not sure about this */
	LandscapeFragment->CastShadow = true;
	/* not sure about this */
	LandscapeFragment->bCastDynamicShadow = true;
	
	LandscapeFragment->bCastStaticShadow = false;
	LandscapeFragment->BodyInstance.bAutoWeld = false;
	LandscapeFragment->SetGenerateOverlapEvents(false);
}

void ADistantPlanActor::MakeFragments(int8 NumFragments) {

	/* Destroy Scene Children */
	ClearScene();

	for (int8 i = 0; i < NumFragments; ++i)
	{
		UStaticMeshComponent* Fragment = NewObject<UStaticMeshComponent>(Scene);

	
		//SetupFragment(Fragment);
		Fragment->CreationMethod = EComponentCreationMethod::Native;
		Fragment->SetMobility(EComponentMobility::Movable);

		Fragment->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
		Fragment->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Fragment->SetWorldScale3D(FVector(1.0f));
	}

	UE_LOG(LogTemp, Warning, TEXT("Scene num children: %d"), Scene->GetNumChildrenComponents());


}

void  ADistantPlanActor::SetFragmentsRules(const TArray<FLandscapeFragmentRules>& Rules)
{
	TArray<USceneComponent*> SceneChildren;
	Scene->GetChildrenComponents(true, SceneChildren);

	

	for (int32 Index = 0; Index < SceneChildren.Num(); ++Index) {

		USceneComponent* Child = SceneChildren[Index];
		UStaticMeshComponent* Fragment = static_cast<UStaticMeshComponent*>(Child);
		FLandscapeFragmentRules FragmentRules = Rules[Index];

		if (::IsValid(Fragment) && ::IsValid(FragmentRules.StaticMesh) )
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Fragment rules StaticMesh to Fragment."));
			Fragment->SetStaticMesh(FragmentRules.StaticMesh);
			Fragment->RegisterComponent();
			
		}
	}

	//this->Register();

	//Scene->RegisterComponent();
	//DrawDebugComponents();
}

void  ADistantPlanActor::SetupLandscapesRulesWithHelpers()
{
	//TArray<FLandscapeFragmentRules> AlpineMountain_A_Fragments = SetupAlphineMountainA();
	
	FString MOUNTAIN_PATH = "/Game/PhotoR_Backgrounds/Meshes/360d_Horizon";

	TArray<FLandscapeFragmentRules> FragmentsA = MakeAlpineMountain(MOUNTAIN_PATH, FString("A"));
	//UE_LOG(LogTemp, Warning, TEXT("Mountain A fragments length: %d"), FragmentsA.Num());
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_A, FragmentsA);

	TArray<FLandscapeFragmentRules> FragmentsB = MakeAlpineMountain(MOUNTAIN_PATH, FString("B"));
	//UE_LOG(LogTemp, Warning, TEXT("Mountain B fragments length: %d"), FragmentsB.Num());
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_B, FragmentsB);

	TArray<FLandscapeFragmentRules> FragmentsC = MakeAlpineMountain(MOUNTAIN_PATH, FString("C"));
	//UE_LOG(LogTemp, Warning, TEXT("Mountain C fragments length: %d"), FragmentsC.Num());
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_C, FragmentsC);

	TArray<FLandscapeFragmentRules> FragmentsD = MakeAlpineMountain(MOUNTAIN_PATH, FString("D"));
	//UE_LOG(LogTemp, Warning, TEXT("Mountain D fragments length: %d"), FragmentsD.Num());
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_D, FragmentsD);

	TArray<FLandscapeFragmentRules> FragmentsE = MakeAlpineMountain(MOUNTAIN_PATH, FString("E"));
	//UE_LOG(LogTemp, Warning, TEXT("Mountain E fragments length: %d"), FragmentsA.Num());
	LandscapeFragmentsRules.Add(EELandscapeBackground::AlpineMountain_E, FragmentsE);
	
	
}

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

