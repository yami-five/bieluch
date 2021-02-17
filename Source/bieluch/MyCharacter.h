// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseTile.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class UInputComponent;
class USceneComponent;

UCLASS(config = Game)
class BIELUCH_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

		/** First person camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY()
		TSubclassOf<class ABaseTile> SpawnTile;

	UPROPERTY()
		TSubclassOf<class ABaseTile> SpawnPassage;

	UPROPERTY()
		TSubclassOf<class ABaseTile> SpawnWall;

	UFUNCTION(BlueprintCallable)
		void Spawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void GenMaze();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
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
