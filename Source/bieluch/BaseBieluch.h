// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBieluch.generated.h"

UCLASS()
class BIELUCH_API ABaseBieluch : public AActor
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
		int GetNumber();

	static int number;

public:
	// Sets default values for this character's properties
	ABaseBieluch();

	void SetNumber(int number);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};