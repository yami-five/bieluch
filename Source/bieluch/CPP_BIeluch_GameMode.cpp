// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BIeluch_GameMode.h"
#include "Math/UnrealMathUtility.h"

ACPP_BIeluch_GameMode::ACPP_BIeluch_GameMode()
{
	static ConstructorHelpers::FObjectFinder<UClass> BlueprintTileBieluchSpawner(TEXT("Blueprint'/Game/Blueprints/BP_BieluchSpawner.BP_BieluchSpawner_C'"));
	if (BlueprintTileBieluchSpawner.Object) {
		SubBieluchSpawner = (UClass*)BlueprintTileBieluchSpawner.Object;
	}
	static ConstructorHelpers::FObjectFinder<UClass> BlueprintPassage(TEXT("Blueprint'/Game/Blueprints/BP_Passage.BP_Passage_C'"));
	if (BlueprintPassage.Object) {
		SubPassage = (UClass*)BlueprintPassage.Object;
	}
	static ConstructorHelpers::FObjectFinder<UClass> BlueprintWall(TEXT("Blueprint'/Game/Blueprints/BP_Wall.BP_Wall_C'"));
	if (BlueprintWall.Object) {
		SubWall = (UClass*)BlueprintWall.Object;
	}
	static ConstructorHelpers::FObjectFinder<UClass> BlueprintBlood(TEXT("Blueprint'/Game/Blueprints/BP_BloodyTile.BP_BloodyTile_C'"));
	if (BlueprintBlood.Object) {
		SubBlood = (UClass*)BlueprintBlood.Object;
	}
	static ConstructorHelpers::FObjectFinder<UClass> BlueprintCharacter(TEXT("Blueprint'/Game/Blueprints/BP_Character.BP_Character_C'"));
	if (BlueprintCharacter.Object) {
		SubCharacter = (UClass*)BlueprintCharacter.Object;
	}
}

// Called when the game starts or when spawned
void ACPP_BIeluch_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_BIeluch_GameMode::Spawn()
{
	TArray<FCharArray> maze;
	bool mazeCheckTest = false;
	while (!mazeCheckTest)
	{
		//generate maze as a matrix
		 maze = ACPP_BIeluch_GameMode::GenMaze();
		 mazeCheckTest = ACPP_BIeluch_GameMode::CheckMaze(maze);
	}
	//spawn maze 
	ACPP_BIeluch_GameMode::SpawnMaze(maze);
	//spawn player
	ACPP_BIeluch_GameMode::SpawnPlayer(maze);
}

TArray<FCharArray> ACPP_BIeluch_GameMode::GenMaze()
{
	FRandomStream rand;
	rand.GenerateNewSeed();
	char wall = 'X';
	char passage = 'O';
	int32 width = 17;
	int32 height = 17;
	TArray<FIntArray> frontiers;
	TArray<FCharArray> map;
	int32 x = rand.RandRange(0, width);
	int32 y = rand.RandRange(0, height);
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
		index = rand.RandRange(0, frontiers.Num() - 1);
		f = frontiers[index];
		frontiers.RemoveAt(index);

		x = f[2];
		y = f[3];

		if (map[x][y] == wall)
		{
			map[f[0]][f[1]] = passage;
			map[x][y] = passage;
			if (x >= 2 && map[x - 2][y] == wall)
			{
				frontiers.Add(FIntArray());
				frontiers[frontiers.Num() - 1].Add(x - 1);
				frontiers[frontiers.Num() - 1].Add(y);
				frontiers[frontiers.Num() - 1].Add(x - 2);
				frontiers[frontiers.Num() - 1].Add(y);
			}
			if (y >= 2 && map[x][y - 2] == wall)
			{
				frontiers.Add(FIntArray());
				frontiers[frontiers.Num() - 1].Add(x);
				frontiers[frontiers.Num() - 1].Add(y - 1);
				frontiers[frontiers.Num() - 1].Add(x);
				frontiers[frontiers.Num() - 1].Add(y - 2);
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
				frontiers[frontiers.Num() - 1].Add(y + 1);
				frontiers[frontiers.Num() - 1].Add(x);
				frontiers[frontiers.Num() - 1].Add(y + 2);
			}
		}
	}
	return map;
}

void ACPP_BIeluch_GameMode::SpawnPlayer(TArray<FCharArray> maze)
{
	if (SubCharacter)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FRotator rotator = { 0,0,0 };
		FVector PlayerSpawnLoc = FVector(0.0f, 0.0f, 100.0f);
		UWorld* world = GetWorld();
		if (maze[7][7] == 'O')
		{
			PlayerSpawnLoc.X = -221.3;
			PlayerSpawnLoc.Y = -221.3;
		}
		else if (maze[8][7] == 'O')
		{
			PlayerSpawnLoc.X = 221.3;
			PlayerSpawnLoc.Y = -221.3;
		}
		else if (maze[7][8] == 'O')
		{
			PlayerSpawnLoc.X = -221.3;
			PlayerSpawnLoc.Y = 221.3;
		}
		else
		{
			PlayerSpawnLoc.X = 221.3;
			PlayerSpawnLoc.Y = 221.3;
		}
		world->SpawnActor<AMyCharacter>(SubCharacter, PlayerSpawnLoc, rotator, spawnParams);
	}
}

void ACPP_BIeluch_GameMode::SpawnMaze(TArray<FCharArray> maze)
{
	FRandomStream rand;
	rand.GenerateNewSeed();
	int numberOfSpawnedBieluchs = 0;
	if (SubPassage && SubWall && SubBieluchSpawner)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			int possibility = 30;
			FRotator rotator = { 0,0,0 };
			float x = -3318.8;
			float y = -3318.8;
			for (int i = 0; i < 17; i++)
			{
				y = -3318.8;
				for (int j = 0; j < 17; j++)
				{
					FVector spawnLocation = { x,y,0 };

					if (maze[i][j] == 'X')
						world->SpawnActor<ABaseTile>(SubWall, spawnLocation, rotator, spawnParams);
					else
					{
						if (((i==1||i==2) && ((j==1||j==2) && numberOfSpawnedBieluchs == 0)||((j == 15 || j == 16) && numberOfSpawnedBieluchs == 1))|| ((i == 15 || i == 16) && ((j == 1 || j == 2) && numberOfSpawnedBieluchs == 2) || ((j == 15 || j == 16) && numberOfSpawnedBieluchs == 3)))
						{
							++numberOfSpawnedBieluchs;
							world->SpawnActor<ABaseTile>(SubBieluchSpawner, spawnLocation, rotator, spawnParams);
						}
						else
						{
							if (rand.RandRange(0, 2)==1)
							{
								world->SpawnActor<ABaseTile>(SubBlood, spawnLocation, rotator, spawnParams);
							}
							else
							{
								world->SpawnActor<ABaseTile>(SubPassage, spawnLocation, rotator, spawnParams);
							}
						}
					}
					y = y + 442.5;
				}
				x = x + 442.5;
			}
		}
	}
}

bool ACPP_BIeluch_GameMode::CheckMaze(TArray<FCharArray> maze)
{
	for (int i = 0; i < 17; i++)
	{
		if (maze[i][0] != 'X' || maze[i][16] != 'X')
			return false;
		else if (maze[0][i] != 'X' || maze[16][i] != 'X')
			return false;
	}
	FString test = "";
	for (int i = 0; i < 17; i++)
	{
		test += maze[2][i];
	}
	if (test == "XXXXXXXXXXXXXXXXX")
		return false;
	return true;
}