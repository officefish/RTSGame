// Fill out your copyright notice in the Description page of Project Settings.

#include "SeasonLandscapeFragment.h"

USeasonLandscapeFragment::USeasonLandscapeFragment()
	: ULandscapeFragment()
{

	FString Path = "/AccurateDistantPlan";
	FString MaterialName = "M_SeasonBlendAttr.M_SeasonBlendAttr";

	TArray< FStringFormatArg > Args;
	Args.Add(FStringFormatArg(Path));
	Args.Add(FStringFormatArg(MaterialName));
	
	FString DemiSeason_M_Source = FString::Format(TEXT("{0}/{1}"), Args);
	static ConstructorHelpers::FObjectFinder<UMaterial> DemiSeason_M_Found(*DemiSeason_M_Source);

	if (DemiSeason_M_Found.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DemiSeason found"));
		
		BlendMaterialInterface = DemiSeason_M_Found.Object;

		BlendMaterialInst = UMaterialInstanceDynamic::Create(BlendMaterialInterface, NULL);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Demi season nor found"));
	}

	if (::IsValid(BlendMaterialInst))
	{
		//BlendMaterialInst = UMaterialInstanceDynamic::Create(BlendMaterialInterface, NULL);
		UE_LOG(LogTemp, Warning, TEXT("BlendMaterialInterface created in USeasonLandscapeFragment::BeginPlay"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("BlendMaterialInterface not init"));
	}

	// Temp exp
	FString TexturePath = "/Game/PhotoR_Backgrounds/Textures";
	FString TextureName = "grass_spring_a_d.grass_spring_a_d";

	TArray< FStringFormatArg > TextureArgs;
	TextureArgs.Add(FStringFormatArg(TexturePath));
	TextureArgs.Add(FStringFormatArg(TextureName));

	FString Exp_T_Source = FString::Format(TEXT("{0}/{1}"), TextureArgs);
	ConstructorHelpers::FObjectFinder<UTexture> Exp_T_Found(*Exp_T_Source);

	if (Exp_T_Found.Succeeded())
	{
		//TempTexture 
		TempTexture = Exp_T_Found.Object;

		
	}


}

void USeasonLandscapeFragment::BeginPlay()
{
	Super::BeginPlay();

	/*if (::IsValid(BlendMaterialInterface))
	{
		BlendMaterialInst = UMaterialInstanceDynamic::Create(BlendMaterialInterface, NULL);
		UE_LOG(LogTemp, Warning, TEXT("BlendMaterialInterface created in USeasonLandscapeFragment::BeginPlay"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("BlendMaterialInterface not init"));
	}*/

}

void USeasonLandscapeFragment::SetRulesAdditions()
{
	if (::IsValid(MeshComponent) && 
		::IsValid(BlendMaterialInst))
	{
		MeshComponent->SetMaterial(0, BlendMaterialInst);
		UE_LOG(LogTemp, Warning, TEXT("Set BlendMaterial for SeasonLandscapeFragment"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Do not set BlendMaterial for SeasonLandscapeFragment"));
	}
}

void USeasonLandscapeFragment::ChangeSeason(TEnumAsByte<EEDemiSeason> Season)
{
	//UE_LOG(LogTemp, Warning, TEXT(" USeasonLandscapeFragment::ChangeSeason"));

	if (::IsValid(MeshComponent) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComponent invalid"));
		return;
	}

	if (::IsValid(BlendMaterialInst) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlendMaterialInst invalid"));
		return;
	}
	
	switch (Season) {
	case EEDemiSeason::Summer:
		{
			SetupSummer(LandscapeRules);
			break;
		}
	case EEDemiSeason::SummerToAutumn:
		{
			SetupSummerToAutumn(LandscapeRules);
			break;
		}
	case EEDemiSeason::Autumn:
		{	
			SetupAutumn(LandscapeRules);
			break;
		}

	case EEDemiSeason::AutumnToWinter:
		{
			SetupAutumnToWinter(LandscapeRules);
			break;
		}

	case EEDemiSeason::Winter:
		{
			SetupWinter(LandscapeRules);
			break;
		}
	
	case EEDemiSeason::WinterToSpring:
		{
			SetupWinterToSpring(LandscapeRules);
			break;
		}
	case EEDemiSeason::Spring:
		{
			SetupSpring(LandscapeRules);
			break;
		}
	case EEDemiSeason::SpringToSummer:
	{
		SetupSpringToSummer(LandscapeRules);
		break;
	}
	}
}

//
void USeasonLandscapeFragment::SmoothChangeSeason(float Delta)
{
	if (::IsValid(MeshComponent) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComponent invalid"));
		return;
	}

	if (::IsValid(BlendMaterialInst) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlendMaterialInst invalid"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("SmoothChangeSeason delta: %f"), Delta);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("BlendDelta")), Delta);
}


void USeasonLandscapeFragment::SetupSummer(FLandscapeFragmentRules Rules)
{
	UE_LOG(LogTemp, Warning, TEXT("Customise Summer textures"));

	// BaseColor-s
	// Red_1
	if (::IsValid(Rules.Summer_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Summer_BC_Red_1_T);
	}
	
	// Red_2
	if (::IsValid(Rules.Summer_BC_Red_2_T))
	{ 
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Summer_BC_Red_2_T);
	}
	
	// Green
	if (::IsValid(Rules.Summer_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Summer_BC_Green_T);	
	}
	
	// Blue
	if (::IsValid(Rules.Summer_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Summer_BC_Blue_T);
	}

	// Normals
	// Normal Red_1
	if (::IsValid(Rules.Summer_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Summer_Normal_Red_1_T);

	}
	// Normal Red_2
	if (::IsValid(Rules.Summer_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Summer_Normal_Red_2_T);
	}
	
	// Normal Green
	if (::IsValid(Rules.Summer_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Summer_Normal_Green_T);

	}
	
	// Normal Blue
	if (::IsValid(Rules.Summer_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Summer_Normal_Blue_T);
	}

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_A")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Splat_T);
	}
	
	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_A")), 0.0f);

	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("WithSnow_B")), 0.0f);

	/* Does not matter*/
	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("WithBlackCrones_A")), 0.0f);

	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("WithBlackCrones_B")), 0.0f);

	// BlendDelta should be 0
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("BlendDelta")), 0.0f);
}

void USeasonLandscapeFragment::SetupSummerToAutumn(FLandscapeFragmentRules Rules)
{
	UE_LOG(LogTemp, Warning, TEXT("Customise Summer To Autumn textures"));

	// BaseColor-s A
	// Red_1
	if (::IsValid(Rules.Summer_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Summer_BC_Red_1_T);
	}

	// Red_2
	if (::IsValid(Rules.Summer_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Summer_BC_Red_2_T);
	}
	
	// Green
	if (::IsValid(Rules.Summer_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Summer_BC_Green_T);
	}
	
	// Blue
	if (::IsValid(Rules.Summer_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Summer_BC_Blue_T);
	}

	// Normals A
	// Normal Red_1
	if (::IsValid(Rules.Summer_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Summer_Normal_Red_1_T);
	}
	
	// Normal Red_2
	if (::IsValid(Rules.Summer_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Summer_Normal_Red_2_T);
	}
	
	// Normal Green
	if (::IsValid(Rules.Summer_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Summer_Normal_Green_T);
	}
	
	// Normal Blue
	if (::IsValid(Rules.Summer_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Summer_Normal_Blue_T);
	}

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_A")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Splat_T);
	}

	// BaseColor-s B
	// Red_1
	if (::IsValid(Rules.Autumn_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_B")), Rules.Autumn_BC_Red_1_T);
	}
	
	// Red_2
	if (::IsValid(Rules.Autumn_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_B")), Rules.Autumn_BC_Red_2_T);
	}
	// Green
	if (::IsValid(Rules.Autumn_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_B")), Rules.Autumn_BC_Green_T);
	}
	// Blue
	if (::IsValid(Rules.Autumn_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_B")), Rules.Autumn_BC_Blue_T);
	}

	// Normals B
	// Normal Red_1
	if (::IsValid(Rules.Autumn_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_B")), Rules.Autumn_Normal_Red_1_T);
	}

	// Normal Red_2
	if (::IsValid(Rules.Autumn_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_B")), Rules.Autumn_Normal_Red_2_T);
	}

	// Normal Green
	if (::IsValid(Rules.Autumn_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_B")), Rules.Autumn_Normal_Green_T);
	}
	
	// Normal Blue
	if (::IsValid(Rules.Autumn_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_B")), Rules.Autumn_Normal_Blue_T);
	}


	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_B")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_B")), Rules.Splat_T);
	}
	
	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_A")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_B")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_A")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_B")), 0.0f);

	// BlendDelta should be 0
	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("BlendDelta")), 0.0f);
}

void USeasonLandscapeFragment::SetupAutumn(FLandscapeFragmentRules Rules)
{
	UE_LOG(LogTemp, Warning, TEXT("Customise Autumn textures"));

	// BaseColor-s B
	// Red_1
	if (::IsValid(Rules.Autumn_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Autumn_BC_Red_1_T);
	}
	
	// Red_2
	if (::IsValid(Rules.Autumn_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Autumn_BC_Red_2_T);
	}

	// Green
	if (::IsValid(Rules.Autumn_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Autumn_BC_Green_T);
	}

	// Blue
	if (::IsValid(Rules.Autumn_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Autumn_BC_Blue_T);
	}

	// Normals B
	// Normal Red_1
	if (::IsValid(Rules.Autumn_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Autumn_Normal_Red_1_T);
	}
	
	// Normal Red_2
	if (::IsValid(Rules.Autumn_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Autumn_Normal_Red_2_T);
	}
	
	// Normal Green
	if (::IsValid(Rules.Autumn_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Autumn_Normal_Green_T);
	}

	// Normal Blue
	if (::IsValid(Rules.Autumn_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Autumn_Normal_Blue_T);
	}

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Main_Normal_A")), Rules.Main_Normal_T);
	}
	
	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Splat_T);
	}

	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_A")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_B")), 0.0f);

	// BlendDelta should be 0
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("BlendDelta")), 0.0f);
}

void USeasonLandscapeFragment::SetupAutumnToWinter(FLandscapeFragmentRules Rules)
{
	UE_LOG(LogTemp, Warning, TEXT("Customise Autumn to Winter textures"));

	// BaseColor-s A
	// Red_1
	if (::IsValid(Rules.Autumn_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Autumn_BC_Red_1_T);
	}

	// Red_2
	if (::IsValid(Rules.Autumn_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Autumn_BC_Red_2_T);
	}
	
	// Green
	if (::IsValid(Rules.Autumn_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Autumn_BC_Green_T);
	}
	
	// Blue
	if (::IsValid(Rules.Autumn_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Autumn_BC_Blue_T);
	}

	// Normals A
	// Normal Red_1
	if (::IsValid(Rules.Autumn_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Autumn_Normal_Red_1_T);
	}
	
	// Normal Red_2
	if (::IsValid(Rules.Autumn_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Autumn_Normal_Red_2_T);
	}

	// Normal Green
	if (::IsValid(Rules.Autumn_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Autumn_Normal_Green_T);
	}
	
	// Normal Blue
	if (::IsValid(Rules.Autumn_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Autumn_Normal_Blue_T);
	}

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_A")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Splat_T);
	}
	
	// BaseColor-s B
	
	if (::IsValid(Rules.Winter_BC_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_B")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_B")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_B")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_B")), Rules.Winter_BC_T);
	}
	
	// Snow
	if (::IsValid(Rules.Winter_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Snow_B")), Rules.Winter_Snow_T);
	}
	
	// Normals B
	// Normal Rock
	if (::IsValid(Rules.Winter_BC_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_B")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_B")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_B")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_B")), Rules.Winter_BC_Normal_T);
	}
	
	// Normal Snow
	if (::IsValid(Rules.Winter_Snow_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Snow_B")), Rules.Winter_Snow_Normal_T);
	}
	
	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_B")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Winter_Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_B")), Rules.Winter_Splat_T);
	}
	
	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_A")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_B")), 1.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_A")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_B")), 1.0f);

}

void USeasonLandscapeFragment::SetupWinter(FLandscapeFragmentRules Rules)
{
	UE_LOG(LogTemp, Warning, TEXT("Customise Winter textures"));

	// BaseColor-s A
	// Rock
	if (::IsValid(Rules.Winter_BC_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Winter_BC_T);

	}
	
	// Snow
	if (::IsValid(Rules.Winter_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Snow_A")), Rules.Winter_Snow_T);
	}

	// Normals B
	// Normal Rock
	if (::IsValid(Rules.Winter_BC_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Winter_BC_Normal_T);
	}
	
	// Normal Snow
	if (::IsValid(Rules.Winter_Snow_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Snow_A")), Rules.Winter_Snow_Normal_T);
	}
	
	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_A")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Winter_Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Winter_Splat_T);
	}

	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_A")), 1.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_A")), 1.0f);

	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("WithSnow_B")), 1.0f);

	// BlendDelta should be 0
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("BlendDelta")), 0.0f);
}

void USeasonLandscapeFragment::SetupWinterToSpring(FLandscapeFragmentRules Rules)
{
	UE_LOG(LogTemp, Warning, TEXT("Customise Winter to Spring textures"));

	// BaseColor-s A
	// Rock
	if (::IsValid(Rules.Winter_BC_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Winter_BC_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Winter_BC_T);
	}
	// Snow
	if (::IsValid(Rules.Winter_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Snow_A")), Rules.Winter_Snow_T);
	}
	
	// Normals B
	// Normal Rock
	if (::IsValid(Rules.Winter_BC_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Winter_BC_Normal_T);

		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Winter_BC_Normal_T);
	}
	
	// Normal Snow
	if (::IsValid(Rules.Winter_Snow_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Snow_A")), Rules.Winter_Snow_Normal_T);
	}

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_A")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Winter_Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Winter_Splat_T);
	}
	
	// BaseColor-s B
	// Red_1
	if (::IsValid(Rules.Spring_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_B")), Rules.Spring_BC_Red_1_T);
	}
	
	// Red-2
	if (::IsValid(Rules.Spring_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_B")), Rules.Spring_BC_Red_2_T);
	}
	
	// Green
	if (::IsValid(Rules.Spring_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_B")), Rules.Spring_BC_Green_T);
	}
	
	// Blue
	if (::IsValid(Rules.Spring_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_B")), Rules.Spring_BC_Blue_T);
	}
	

	// Snow
	if (::IsValid(Rules.Spring_BC_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Snow_B")), Rules.Spring_BC_Snow_T);
	}
	
	// Normals B
	// Normal Red-1
	if (::IsValid(Rules.Spring_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_B")), Rules.Spring_Normal_Red_1_T);
	}
	
	// normal red-2
	if (::IsValid(Rules.Spring_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_B")), Rules.Spring_Normal_Red_2_T);
	}
	
	// normal green
	if (::IsValid(Rules.Spring_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_B")), Rules.Spring_Normal_Green_T);
	}

	// normal blue
	if (::IsValid(Rules.Spring_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_B")), Rules.Spring_Normal_Blue_T);
	}
	

	// Normal Snow
	if (::IsValid(Rules.Spring_Normal_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Snow_B")), Rules.Spring_Normal_Snow_T);
	}
	

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_B")), Rules.Main_Normal_T);
	}


	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_B")), Rules.Splat_T);
	}

	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_A")), 1.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_A")), 1.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_B")), 1.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_B")), 0.0f);

	// BlendDelta should be 0
	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("BlendDelta")), 0.0f);
}

void USeasonLandscapeFragment::SetupSpring(FLandscapeFragmentRules Rules)
{
	// BaseColor-s B
	// Red_1
	if (::IsValid(Rules.Spring_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Spring_BC_Red_1_T);
	}

	// Red-2
	if (::IsValid(Rules.Spring_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Spring_BC_Red_2_T);
	}

	// Green
	if (::IsValid(Rules.Spring_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Spring_BC_Green_T);
	}

	// Blue
	if (::IsValid(Rules.Spring_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Spring_BC_Blue_T);
	}


	// Snow
	if (::IsValid(Rules.Spring_BC_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Snow_A")), Rules.Spring_BC_Snow_T);
	}

	// Normals B
	// Normal Red-1
	if (::IsValid(Rules.Spring_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Spring_Normal_Red_1_T);
	}

	// normal red-2
	if (::IsValid(Rules.Spring_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Spring_Normal_Red_2_T);
	}

	// normal green
	if (::IsValid(Rules.Spring_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Spring_Normal_Green_T);
	}

	// normal blue
	if (::IsValid(Rules.Spring_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Spring_Normal_Blue_T);
	}


	// Normal Snow
	if (::IsValid(Rules.Spring_Normal_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Snow_A")), Rules.Spring_Normal_Snow_T);
	}


	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_A")), Rules.Main_Normal_T);
	}


	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Splat_T);
	}

	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithWinter_A")), 1.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_A")), 0.0f);

	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("WithWinter_B")), 1.0f);

	// BlendDelta should be 0
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("BlendDelta")), 0.0f);
}

void USeasonLandscapeFragment::SetupSpringToSummer(FLandscapeFragmentRules Rules)
{
	UE_LOG(LogTemp, Warning, TEXT("Customise Spring to Summer textures"));

	// BaseColor-s A
	// Red 1
	if (::IsValid(Rules.Spring_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_A")), Rules.Spring_BC_Red_1_T);
	}
	// Red 2
	if (::IsValid(Rules.Spring_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_A")), Rules.Spring_BC_Red_2_T);
	}
	// Green
	if (::IsValid(Rules.Spring_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_A")), Rules.Spring_BC_Green_T);
	}
	// Blue
	if (::IsValid(Rules.Spring_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_A")), Rules.Spring_BC_Blue_T);
	}
	// Snow
	if (::IsValid(Rules.Spring_BC_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Snow_A")), Rules.Spring_BC_Snow_T);
	}

	// Normals A
	// Normal Red 1
	if (::IsValid(Rules.Spring_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_A")), Rules.Spring_BC_Red_1_T);
	}
	// Normal red 2
	if (::IsValid(Rules.Spring_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_A")), Rules.Spring_BC_Red_2_T);
	}
	// Normal green
	if (::IsValid(Rules.Spring_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_A")), Rules.Spring_BC_Green_T);
	}
	// Normal blue 
	if (::IsValid(Rules.Spring_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_A")), Rules.Spring_BC_Blue_T);
	}
	// Normal Snow
	if (::IsValid(Rules.Spring_Normal_Snow_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Snow_A")), Rules.Spring_Normal_Snow_T);
	}

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_A")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_A")), Rules.Splat_T);
	}

	// BaseColor-s B
	// Red_1
	if (::IsValid(Rules.Summer_BC_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_1_B")), Rules.Summer_BC_Red_1_T);
	}

	// Red-2
	if (::IsValid(Rules.Summer_BC_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Red_2_B")), Rules.Summer_BC_Red_2_T);
	}

	// Green
	if (::IsValid(Rules.Summer_BC_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Green_B")), Rules.Summer_BC_Green_T);
	}

	// Blue
	if (::IsValid(Rules.Summer_BC_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("BaseColor_Blue_B")), Rules.Summer_BC_Blue_T);
	}


	// Normals B
	// Normal Red-1
	if (::IsValid(Rules.Summer_Normal_Red_1_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_1_B")), Rules.Summer_Normal_Red_1_T);
	}

	// normal red-2
	if (::IsValid(Rules.Summer_Normal_Red_2_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Red_2_B")), Rules.Summer_Normal_Red_2_T);
	}

	// normal green
	if (::IsValid(Rules.Summer_Normal_Green_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Green_B")), Rules.Summer_Normal_Green_T);
	}

	// normal blue
	if (::IsValid(Rules.Summer_Normal_Blue_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("Normal_Blue_B")), Rules.Summer_Normal_Blue_T);
	}

	// Main Normal 
	if (::IsValid(Rules.Main_Normal_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("MainNormal_B")), Rules.Main_Normal_T);
	}

	// SPLAT
	if (::IsValid(Rules.Splat_T))
	{
		BlendMaterialInst->SetTextureParameterValue(
			FName(TEXT("SPLAT_B")), Rules.Splat_T);
	}

	// With winter A / B false
	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_A")), 1.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_A")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithSnow_B")), 0.0f);

	BlendMaterialInst->SetScalarParameterValue(
		FName(TEXT("WithBlackCrones_B")), 0.0f);

	// BlendDelta should be 0
	//BlendMaterialInst->SetScalarParameterValue(
	//	FName(TEXT("BlendDelta")), 0.0f);
}