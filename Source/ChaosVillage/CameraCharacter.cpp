// Fill out your copyright notice in the Description page of Project Settings.

/*
************* RTS/GOD SIM CAMERA *************
Player will be an invisible centerpoint on the screen.

W/S for +-Vertical
A/D for +-Horizontal
Scroll Wheel for zoom
Hold right mouse for rotation around centerpoint.

float CameraSpeed = 5f;
float CameraRotationSpeed = 10f

	// Camera Movement
	switch CamMovement
	W:
		Player.Camera.Velocity.Y = CameraSpeed;
		break;
	S:
		Player.Camera.Velocity.Y = -CameraSpeed;
		break;
	A:
		Player.Camera.Velocity.X = CameraSpeed;
		break;
	D:
		Player.Camera.Velocity.X = -CameraSpeed;
		break;
	default:
		Player.Camera.Velocity.X = 0;
		break;

	// Camera Zoom TODO: I want it to move in both the z and whichever other axis is needed to zoom, so I'll have to figure out the easiest way to do that
	if(Keystroke == +scrollwheel)
		Player.Camera.transform = Amount;
	if(Keystroke == -scrollwheel)
		Player.Camera.transform = -Amount;

	// Camera Rotation
	if(KeyPressedAndHeld == RightMouse && Mouse.Moved)
		if(Mouse.Moved = RightSwipe)
			Camera.Rotate = CameraRotationSpeed;
		if(Mouse.Moved = LeftSwipe)
			Camera.Rotate = -CameraRotationSpeed;

*/

#include "CameraCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACameraCharacter::ACameraCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating your class Components
	CameraRShoulderLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraRShoulderLocation"));
	CameraOriginLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraOriginLocation"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	// Attaching your class Components to the default character's Skeletal Mesh Component.
	CameraRShoulderLocation->SetupAttachment(GetMesh());
	CameraOriginLocation->SetupAttachment(GetMesh());
	CameraComp->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void ACameraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

