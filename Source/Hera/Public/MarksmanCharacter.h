// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MarksmanCharacter.generated.h"

UCLASS()
class HERA_API AMarksmanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMarksmanCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class USceneComponent* SceneComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputDataAsset* InputDataAsset;

	void MoveAction(const struct FInputActionValue& Value);
	void MoveStartedAction(const FInputActionValue& Value); 
	void MoveCompletedAction(const struct FInputActionValue& Value);
	
	APlayerController* PlayerController;
	UCharacterMovementComponent* CharacterMovementComponent;

private:
	FVector LastClickedLocation;
	bool MoveStarted;
	bool IsSprinting;
	float MaxWalkSpeed = 300.f;
	float DefaultWalkSpeed = 200.f;
	float InterpolationSpeed = 4.f;
	float MoveElapsedSeconds;
};
