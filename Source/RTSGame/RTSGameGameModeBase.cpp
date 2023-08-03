// Copyright Epic Games, Inc. All Rights Reserved.


#include "RTSGameGameModeBase.h"

ARTSGameGameModeBase::ARTSGameGameModeBase()
	:
	AGameModeBase()
{
	/* Setup default Base classes */
	SetupDefaults();

	/* Create ProjectSettings Proxy */
	//ProjectSettings = NewObject<UProjectSettingsRuntime>();
}

void ARTSGameGameModeBase::SetupDefaults()
{
	/* Set RTS Player Controller as default */
	PlayerControllerClass = ARTSPlayerController::StaticClass();
	
	/* Set RTSSmoothCameraPawn as default */
	DefaultPawnClass = AAccurateDayNightPawn::StaticClass();

	/* Set CultureStateBase as default */
	GameStateClass = ACultureGameStateBase::StaticClass();


}