// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseTile.h"
#include "MyCharacter.h"
#include "BaseBieluch.h"
#include "CPP_BIeluch_GameMode.generated.h"

UCLASS()
class BIELUCH_API ACPP_BIeluch_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	UPROPERTY()
		TSubclassOf<class ABaseTile> SubTile;

	UPROPERTY()
		TSubclassOf<class ABaseTile> SubPassage;

	UPROPERTY()
		TSubclassOf<class ABaseTile> SubWall;

	UPROPERTY()
		TSubclassOf<class ABaseBieluch> SubBieluch;

	UPROPERTY()
		TSubclassOf<class AMyCharacter> SubCharacter;

	UFUNCTION(BlueprintCallable)
		void Spawn();

public:
	ACPP_BIeluch_GameMode();

	TArray<FCharArray> GenMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnMaze(TArray<FCharArray> maze);

	void SpawnPlayer(TArray<FCharArray> maze);

	TArray<FVector> bieluchsLocations;
};

USTRUCT()
struct FIntArray {
	GENERATED_BODY()
public:

	TArray<int32> Ints;

	int32 operator[] (int32 i) {
		return Ints[i];
	}

	void Add(int32 number) {
		Ints.Add(number);
	}
};

USTRUCT()
struct FCharArray {
	GENERATED_BODY()
public:

	TArray<char> Chars;

	char& operator[] (int32 i) {
		return Chars[i];
	}

	void Add(char chr) {
		Chars.Add(chr);
	}
};
