// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RTSSmoothCameraPawn.h"
#include "CultureGameStateBase.h"

#include "AccurateDayNightPawn.generated.h"

/**
 * 
 */
UCLASS()
class RTSGAME_API AAccurateDayNightPawn : public ARTSSmoothCameraPawn
{
	GENERATED_BODY()

public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void GameSpeedUp();
	void GameSpeedDown();
	ACultureGameStateBase* GetGameState() const;
	
};
