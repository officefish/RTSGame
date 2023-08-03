// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CultureGameStateBase.h"

#include "AccurateDayNightPawn.h"
#include "RTSPlayerController.h"

#include "RTSGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSGAME_API ARTSGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARTSGameGameModeBase();

private:
	void SetupDefaults();
};
