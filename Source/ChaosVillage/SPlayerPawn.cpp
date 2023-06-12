// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASPlayerPawn::ASPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating the root component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	// Create the SpringArm, Set it to the root, give it a length, and turn off the collision
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent -> SetupAttachment(RootComponent);
	SpringArmComponent -> TargetArmLength = 2000.0f;
	SpringArmComponent -> bDoCollisionTest = false;

	// Create the CameraComponent, and attach it to the SpringArm
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent -> SetupAttachment(SpringArmComponent);
	
}

// Called when the game starts or when spawned
void ASPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Set initial zoom and location
	TargetLocation = GetActorLocation();
	TargetZoom = 3000.0f;

	// Set initial camera rotation
	const FRotator Rotation = SpringArmComponent -> GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch + -50, Rotation.Yaw, 0.0f);
}

void ASPlayerPawn::Forward(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	
	// Find TargetLocation with ForwardVector
	TargetLocation = SpringArmComponent->GetForwardVector() * AxisValue * MoveSpeed + TargetLocation;
}

void ASPlayerPawn::Right(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	// Find TargetLocation with RightVector
	TargetLocation = SpringArmComponent->GetRightVector() * AxisValue * MoveSpeed + TargetLocation;
}

void ASPlayerPawn::Zoom(float AxisValue)
{

	if (AxisValue == 0.0f)
	{
		return;
	}

	// Give a constant float value and then Clamp the zoom between it's Min/Max Settings
	const float Zoom = AxisValue * 100.0f;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, ZoomMin, ZoomMax);
}

void ASPlayerPawn::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, -45, 0.0f));
}

void ASPlayerPawn::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, 45, 0.0f));
}

void ASPlayerPawn::RotateEnable()
{
	CanRotate = true;
}

void ASPlayerPawn::RotateDisable()
{
	CanRotate = false;
}

void ASPlayerPawn::RotateHorizontal(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, AxisValue, 0.0f));
	}
	
}

void ASPlayerPawn::RotateVertical(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(AxisValue, 0.0f, 0.0f));
	}
}

void ASPlayerPawn::CameraBounds()
{
	float NewPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch < (RotatePitchMax * -1))
	{
		NewPitch = (RotatePitchMax * -1);
	}
	else if (TargetRotation.Pitch > (RotatePitchMin * -1))
	{
		NewPitch = (RotatePitchMin * -1);
	}
	
	// Set the new pitch and clamp any roll
	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0.0f);

	// Clamp desired location to winith bounds
	TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0.0f);
}

// Called every frame
void ASPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraBounds();

	// Move the pawn
	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpolatedLocation);

	// Zoom the camera
	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent -> TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	SpringArmComponent -> TargetArmLength = InterpolatedZoom;

	//Rotate the camera
	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(SpringArmComponent -> GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	SpringArmComponent -> SetRelativeRotation(InterpolatedRotation);

}

// Called to bind functionality to input
void ASPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Adding the keybinds to the functions themselves.
	// Component points to BindAxis(InputIMade), ObjectThisAffects, &ObjectName::UFUNCTION
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ASPlayerPawn::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &ASPlayerPawn::Right);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &ASPlayerPawn::Zoom);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &ASPlayerPawn::RotateHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &ASPlayerPawn::RotateVertical);

	//Component points to BindAction(InputIMade), WhatAction, ObjectThisAffects, &ObjectName::UFUNCTION
	PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed, this, &ASPlayerPawn::RotateRight);
	PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed, this, &ASPlayerPawn::RotateLeft);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Pressed, this, &ASPlayerPawn::RotateEnable);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Released, this, &ASPlayerPawn::RotateDisable);

}

