// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "InputDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class HERA_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputMove;
 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputJump;
};
