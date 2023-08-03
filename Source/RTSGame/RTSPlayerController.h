// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "CultureGameStateBase.h"
#include "AccurateDayNightPawn.h"

#include "RTSPlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RTSGAME_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARTSPlayerController();

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Editor Mode")
		bool bDebugMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Editor Mode")
		bool bMoveWithCursor;

protected:

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	
	ACultureGameStateBase* GetGameState() const;
};
