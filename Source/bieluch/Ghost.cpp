// Fill out your copyright notice in the Description page of Project Settings.


#include "Ghost.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGhost::AGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGhost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGhost::Move(FVector targetLocation)
{
	FVector GhostLocation = GetActorLocation();
	targetLocation *= 1.0;
	FRotator GhostRotator = UKismetMathLibrary::FindLookAtRotation(GhostLocation, targetLocation);
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GhostRotator);
	ForwardVector *= 20;
	ForwardVector = { ForwardVector.X, ForwardVector.Y, 0.0 };
	AddActorWorldOffset(ForwardVector);
	SetActorRotation(UKismetMathLibrary::MakeRotFromX(ForwardVector));
}