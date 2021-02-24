// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBieluch.h"
int ABaseBieluch::number = 0;
// Sets default values
ABaseBieluch::ABaseBieluch()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseBieluch::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseBieluch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int  ABaseBieluch::GetNumber()
{
	return number;
}

void  ABaseBieluch::SetNumber(int _number)
{
	number=_number;
}