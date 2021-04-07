// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	static ConstructorHelpers::FObjectFinder<UClass> BlueprintGhost(TEXT("Blueprint'/Game/Blueprints/BP_Ghost.BP_Ghost_C'"));
	if (BlueprintGhost.Object) {
		SubGhost = (UClass*)BlueprintGhost.Object;
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ghostTimer = 0;
	ghostIsSpawned = false;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);
}

void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AMyCharacter::SpawnOrDestroyGhost(int GainedBieluchs=0)
{
	if (ghostIsSpawned == false && ghostTimer >= (45 - 5 * GainedBieluchs)*10)
	{
		ghostIsSpawned = true;
		float tick = GetActorTickInterval();
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FRotator rotator = GetActorRotation();
		rotator.Pitch = 0.0;
		rotator.Roll = 0.0;
		FVector location = GetActorLocation();
		location += GetActorForwardVector() * (500 - GainedBieluchs * 10);
		location.Z = 0.0;
		UWorld* world = GetWorld();
		CPP_Ghost = world->SpawnActor<AGhost>(SubGhost, location, rotator, spawnParams);
		ghostTimer = 0;
	}
	else if (ghostIsSpawned == false && ghostTimer >=0)
	{
		ghostTimer++;
	}
	else
	{
		FVector PlayerLoc = GetActorLocation();
		FVector GhostLoc = CPP_Ghost->GetActorLocation();
		if ((UKismetMathLibrary::Distance2D({ PlayerLoc.X, PlayerLoc.Y }, { GhostLoc.X, GhostLoc.Y }) <= 100-10*GainedBieluchs) || ghostTimer>=350)
		{
			CPP_Ghost->Destroy();
			ghostIsSpawned = false;
			ghostTimer = 0;
		}
		else
		{
			ghostTimer++;
			CPP_Ghost->Move(PlayerLoc);
		}
	}
}