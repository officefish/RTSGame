// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSSmoothCameraComponent.h"
#include "RTSSmoothCameraPawn.generated.h"

UCLASS()
class RTSSMOOTHCAMERA_API ARTSSmoothCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSSmoothCameraPawn();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SmoothCamera")
		URTSSmoothCameraComponent* RTSCameraComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	void CameraPanPressed();
	void CameraPanReleased();

	void MoveForwardBackward(float AxisValue);
	void MoveSideways(float AxisValue);

	void CameraKeyboardPan(float AxisValue);
	void CameraKeyboardPitch(float AxisValue);
	void CameraZoom(float AxisValue);
};
