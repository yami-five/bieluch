// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_BIeluch_GameMode.generated.h"

UCLASS()
class BIELUCH_API ACPP_BIeluch_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACPP_BIeluch_GameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
