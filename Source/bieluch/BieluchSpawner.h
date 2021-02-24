// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BieluchSpawner.generated.h"

UCLASS()
class BIELUCH_API ABieluchSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABieluchSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/*FVector bieluchsLocations;

	void init(FVector _bieluchsLocations);*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//static ABieluchSpawner* MAKE(FVector bieluchsLocations);
};
