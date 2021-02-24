// Fill out your copyright notice in the Description page of Project Settings.


#include "BieluchSpawner.h"

// Sets default values
ABieluchSpawner::ABieluchSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABieluchSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABieluchSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//ABieluchSpawner* ABieluchSpawner::MAKE(FVector _bieluchsLocations) 
//{
//	ABieluchSpawner* bs = NewObject<ABieluchSpawner>();
//	bs->init(_bieluchsLocations);
//	return bs;
//}
//
//void ABieluchSpawner::init(FVector _bieluchsLocations)
//{
//	ABieluchSpawner::bieluchsLocations = _bieluchsLocations;
//}