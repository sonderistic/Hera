// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputDataAsset.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AMarksmanCharacter::AMarksmanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AMarksmanCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMarksmanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMarksmanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovementComponent = GetCharacterMovement(); // Rotate character to moving direction
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0.f, 640.f, 0.f);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;

	// Get the player controller
    PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetShowMouseCursor(true);

    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMapping, 0);
    
	// Get the EnhancedInputComponent
    UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    // Bind the actions
    PEI->BindAction(InputDataAsset->InputMove, ETriggerEvent::Triggered, this, &AMarksmanCharacter::MoveAction);
	PEI->BindAction(InputDataAsset->InputMove, ETriggerEvent::Completed, this, &AMarksmanCharacter::MoveCompletedAction);
	PEI->BindAction(InputDataAsset->InputJump, ETriggerEvent::Triggered, this, &AMarksmanCharacter::JumpAction);
}

void AMarksmanCharacter::MoveAction(const FInputActionValue& Value) 
{
	if (!PlayerController)
	{
		return;
	}

	bool IsMove = Value.Get<bool>();
	FHitResult TraceHitResult;
	if (IsMove &&
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult))
	{
		LastClickedLocation = TraceHitResult.Location;
		APawn::AddMovementInput(LastClickedLocation - GetActorLocation());
	}
}

void AMarksmanCharacter::MoveCompletedAction(const FInputActionValue& Value) 
{
	if (!PlayerController)
	{
		return;
	}

	// Move the player to the last-known-clicked location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, LastClickedLocation);
}

void AMarksmanCharacter::JumpAction(const FInputActionValue& Value)
{
	if (!PlayerController)
	{
		return;
	}

	bool IsJump = Value.Get<bool>();
	if (IsJump)
	{

	}
}

