// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerController.h"

ARTSPlayerController::ARTSPlayerController()
	: APlayerController() 
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

    bDebugMode = false;
    bMoveWithCursor = true;
}

void ARTSPlayerController::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    FName PropName = PropertyChangedEvent.GetPropertyName();
   
    if (PropName == GET_MEMBER_NAME_CHECKED(ARTSPlayerController, bDebugMode))
    {
        ACultureGameStateBase* GameState = GetGameState();

        if (::IsValid(GameState) == false) return;

        if (bDebugMode) {
            //UE_LOG(LogTemp, Warning, TEXT("Do something when debug mode"));
            GameState->bWithRealTime = true;
            GameState->bWithX50Time = true;
            GameState->bWithX500Time = true;
        }
        else {
            //UE_LOG(LogTemp, Warning, TEXT("Do something when not debug mode"));
            GameState->bWithRealTime = false;
            GameState->bWithX50Time = false;
            GameState->bWithX500Time = false;
        }
    }

    if (PropName == GET_MEMBER_NAME_CHECKED(ARTSPlayerController, bMoveWithCursor))
    {
        AAccurateDayNightPawn* AccuratePawn = ARTSPlayerController::GetPawn<AAccurateDayNightPawn>();
        if (::IsValid(AccuratePawn) == false) return;

        AccuratePawn->RTSCameraComponent->bMoveWithCursor = false;
    }
}

ACultureGameStateBase* ARTSPlayerController::GetGameState() const
{
    return GetWorld() != NULL ? GetWorld()->GetGameState<ACultureGameStateBase>() : NULL;
}

