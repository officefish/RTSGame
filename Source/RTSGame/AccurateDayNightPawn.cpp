// Fill out your copyright notice in the Description page of Project Settings.


#include "AccurateDayNightPawn.h"

void AAccurateDayNightPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind actions for speed up / down GameState
	PlayerInputComponent->BindAction("GameSpeedUp", IE_Pressed, this, &AAccurateDayNightPawn::GameSpeedUp);
	PlayerInputComponent->BindAction("GameSpeedDown", IE_Pressed, this, &AAccurateDayNightPawn::GameSpeedDown);

}

void AAccurateDayNightPawn::GameSpeedUp()
{
	ACultureGameStateBase* GameState = GetGameState();
	if (::IsValid(GameState)) 
	{
		GameState->GameSpeedUp();
	}
}

void AAccurateDayNightPawn::GameSpeedDown()
{
	ACultureGameStateBase* GameState = GetGameState();
	if (::IsValid(GameState)) 
	{
		GameState->GameSpeedDown();
	}
}

ACultureGameStateBase* AAccurateDayNightPawn::GetGameState() const
{
	return GetWorld() != NULL ? GetWorld()->GetGameState<ACultureGameStateBase>() : NULL;
}