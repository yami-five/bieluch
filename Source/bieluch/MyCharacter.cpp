// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Math/UnrealMathUtility.h"

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

	static ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintTile(TEXT("Blueprint'/Game/Blueprints/BP_Tile.BP_Tile'"));
	if (BlueprintTile.Object) {
		SpawnTile = (UClass*)BlueprintTile.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintPassage(TEXT("Blueprint'/Game/Blueprints/BP_Passage.BP_Passage'"));
	if (BlueprintPassage.Object) {
		SpawnPassage = (UClass*)BlueprintPassage.Object->GeneratedClass;
	}
	static ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintWall(TEXT("Blueprint'/Game/Blueprints/BP_Wall.BP_Wall'"));
	if (BlueprintWall.Object) {
		SpawnWall = (UClass*)BlueprintWall.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

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

void AMyCharacter::Spawn()
{
	TArray<FCharArray> maze=AMyCharacter::GenMaze();

	if (SpawnPassage && SpawnWall)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;

			FRotator rotator = { 0,0,0 };

			float x = 5200.0;
			float y = 5200.0;
			for (int i = 0; i < 17; i++)
			{
				y = 5200.0;
				for (int j = 0; j < 17; j++)
				{
					FVector spawnLocation = { x,y,0 };

					if(maze[i][j]=='X')
						world->SpawnActor<ABaseTile>(SpawnWall, spawnLocation, rotator, spawnParams);
					else
						world->SpawnActor<ABaseTile>(SpawnPassage, spawnLocation, rotator, spawnParams);
					y = y - 650.0;
				}
				x = x - 650.0;
			}
		}
	}
}

TArray<FCharArray> AMyCharacter::GenMaze()
{
	FRandomStream rand;
	char wall = 'X';
	char passage = 'O';
	int32 width = 17;
	int32 height = 17;
	TArray<FIntArray> frontiers;
	TArray<FCharArray> map;
	int32 x= rand.RandRange(0, width);
	int32 y= rand.RandRange(0, height);
	for (int i = 0; i < height; i++)
	{
		map.Add(FCharArray());
		for (int j = 0; j < width; j++)
		{
			map[i].Add(wall);
		}
	}
	frontiers.Add(FIntArray());
	frontiers[0].Add(x);
	frontiers[0].Add(y);
	frontiers[0].Add(x);
	frontiers[0].Add(y);

	FIntArray f;
	int32 index;

	while (frontiers.Num() != 0)
	{
		index = rand.RandRange(0, frontiers.Num()-1);
		f = frontiers[index];
		frontiers.RemoveAt(index);

		x = f[2];
		y = f[3];

		if (map[x][y] == wall)
		{
			map[f[0]][f[1]]=passage;
			map[x][y]=passage;
			if (x >= 2 && map[x - 2][y] == wall)
			{
				frontiers.Add(FIntArray());
				frontiers[frontiers.Num()-1].Add(x-1);
				frontiers[frontiers.Num() - 1].Add(y);
				frontiers[frontiers.Num() - 1].Add(x-2);
				frontiers[frontiers.Num() - 1].Add(y);
			}
			if (y >= 2 && map[x][y - 2] == wall)
			{
				frontiers.Add(FIntArray());
				frontiers[frontiers.Num() - 1].Add(x);
				frontiers[frontiers.Num() - 1].Add(y-1);
				frontiers[frontiers.Num() - 1].Add(x);
				frontiers[frontiers.Num() - 1].Add(y-2);
			}
			if (x < width - 2 && map[x + 2][y] == wall)
			{
				frontiers.Add(FIntArray());
				frontiers[frontiers.Num() - 1].Add(x + 1);
				frontiers[frontiers.Num() - 1].Add(y);
				frontiers[frontiers.Num() - 1].Add(x + 2);
				frontiers[frontiers.Num() - 1].Add(y);

			}
			if (y < height - 2 && map[x][y + 2] == wall) 
			{
				frontiers.Add(FIntArray());
				frontiers[frontiers.Num() - 1].Add(x);
				frontiers[frontiers.Num() - 1].Add(y+1);
				frontiers[frontiers.Num() - 1].Add(x);
				frontiers[frontiers.Num() - 1].Add(y+2);
			}
		}
	}
	return map;
}