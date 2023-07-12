// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSSmoothCameraPawn.h"

// Sets default values
ARTSSmoothCameraPawn::ARTSSmoothCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RTSCameraComponent = CreateDefaultSubobject<URTSSmoothCameraComponent>(TEXT("RTSCameraComponent"));
	if (!ensure(RTSCameraComponent != nullptr)) {
		return;
	}

	RTSCameraComponent->SetupAttachment(this->RootComponent);

}

// Called when the game starts or when spawned
void ARTSSmoothCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARTSSmoothCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARTSSmoothCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &ARTSSmoothCameraPawn::MoveForward);
	PlayerInputComponent->BindAction("MoveBackward", IE_Pressed, this, &ARTSSmoothCameraPawn::MoveBackward);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARTSSmoothCameraPawn::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARTSSmoothCameraPawn::MoveRight);

	PlayerInputComponent->BindAction("CameraPan", IE_Pressed, this, &ARTSSmoothCameraPawn::CameraPanPressed);
	PlayerInputComponent->BindAction("CameraPan", IE_Released, this, &ARTSSmoothCameraPawn::CameraPanReleased);

	PlayerInputComponent->BindAxis("MoveForwardBackward", this, &ARTSSmoothCameraPawn::MoveForwardBackward);
	PlayerInputComponent->BindAxis("MoveSideways", this, &ARTSSmoothCameraPawn::MoveSideways);

	PlayerInputComponent->BindAxis("CameraKeyboardPan", this, &ARTSSmoothCameraPawn::CameraKeyboardPan);
	PlayerInputComponent->BindAxis("CameraKeyboardPitch", this, &ARTSSmoothCameraPawn::CameraKeyboardPitch);
	PlayerInputComponent->BindAxis("CameraZoom", this, &ARTSSmoothCameraPawn::CameraZoom);

}


void ARTSSmoothCameraPawn::MoveForward()
{
	RTSCameraComponent->MoveCameraUpDown(1.0f);
}

void ARTSSmoothCameraPawn::MoveBackward()
{
	RTSCameraComponent->MoveCameraUpDown(-1.0f);
}

void ARTSSmoothCameraPawn::MoveLeft()
{
	RTSCameraComponent->MoveCameraLeftRight(1.0f);
}

void ARTSSmoothCameraPawn::MoveRight()
{
	RTSCameraComponent->MoveCameraLeftRight(-1.0f);
}

void ARTSSmoothCameraPawn::MoveForwardBackward(float AxisValue)
{
	RTSCameraComponent->MoveCameraUpDownPerUnit(AxisValue);
}

void ARTSSmoothCameraPawn::MoveSideways(float AxisValue)
{
	RTSCameraComponent->MoveCameraLeftRightPerUnit(AxisValue);
}

void ARTSSmoothCameraPawn::CameraPanPressed()
{
	RTSCameraComponent->RotateCameraPressed();
}

void ARTSSmoothCameraPawn::CameraPanReleased()
{
	RTSCameraComponent->RotateCameraReleased();
}

void ARTSSmoothCameraPawn::CameraKeyboardPan(float AxisValue)
{
	RTSCameraComponent->PanCamera(AxisValue);
}

void ARTSSmoothCameraPawn::CameraKeyboardPitch(float AxisValue)
{
	RTSCameraComponent->PitchCamera(AxisValue);
}

void ARTSSmoothCameraPawn::CameraZoom(float AxisValue)
{
	RTSCameraComponent->ZoomCamera(AxisValue);
}

