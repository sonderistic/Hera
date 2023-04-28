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
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"

// Sets default values
AMarksmanCharacter::AMarksmanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Don't rotate character to camera direction
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	CharacterMovementComponent = GetCharacterMovement(); // Rotate character to moving direction
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0.f, 640.f, 0.f);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;
}

// Called when the game starts or when spawned
void AMarksmanCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// TODO: Setup player input in a PlayerController
// Called to bind functionality to input
void AMarksmanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	PEI->BindAction(InputDataAsset->InputMove, ETriggerEvent::Canceled, this, &AMarksmanCharacter::MoveCompletedAction);
	PEI->BindAction(InputDataAsset->InputMove, ETriggerEvent::Started, this, &AMarksmanCharacter::MoveStartedAction);
}

// Called every frame
void AMarksmanCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Velocity = GetVelocity();
    Velocity.Z = 0.f;

    if (MoveStarted)
    {
        MoveElapsedSeconds += DeltaTime;
    }

    float SpeedMultiplier = IsSprinting ? 2.5f : 1.f;
    float InterpolatedSpeed = FMath::Lerp(Velocity.Size(), MaxWalkSpeed * SpeedMultiplier, InterpolationSpeed * DeltaTime);
    CharacterMovementComponent->MaxWalkSpeed = InterpolatedSpeed;
    /*
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            2,
            FColor::Yellow,
            FString::Printf(TEXT("Speed %f"), InterpolatedSpeed)
        );
    }
    */
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
        // Trace to the ground to get the exact point clicked
        if (UNavigationSystemV1::GetCurrent(GetWorld()) != nullptr)
        {
            FNavLocation NavLocation;
            if (UNavigationSystemV1::GetCurrent(GetWorld())->ProjectPointToNavigation(TraceHitResult.Location, NavLocation))
            {
                MoveStarted = true;
				LastClickedLocation = NavLocation.Location;
                /*
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(
                        -1,
                        2,
                        FColor::Yellow,
                        FString::Printf(TEXT("Elapsed Seconds %f"), MoveElapsedSeconds)
                    );
                }
                */
                if (MoveElapsedSeconds > 0.1f)
                {
                    IsSprinting = true;
                    // Set the character to move towards that point
                    FVector Direction = (LastClickedLocation - GetActorLocation()).GetSafeNormal();
                    APawn::AddMovementInput(Direction);

                    // Set the character to rotate towards the mouse position
                    FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LastClickedLocation);
                    SetActorRotation(FRotator(0.f, Rot.Yaw, 0.f));
                }
            }
        }

    }
}

void AMarksmanCharacter::MoveStartedAction(const FInputActionValue& Value)
{
    if (!PlayerController)
    {
        return;
    }
    PlayerController->StopMovement();
    // Set the character to move at a constant speed
    CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeed;
}

void AMarksmanCharacter::MoveCompletedAction(const FInputActionValue& Value) 
{
    if (!PlayerController)
    {
        return;
    }

    IsSprinting = false;

    /*
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            2,
            FColor::Yellow,
            FString::Printf(TEXT("Elapsed Seconds End %f"), MoveElapsedSeconds)
        );
    }
    */
    // Move the player to the last-known-clicked location
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, LastClickedLocation);

    MoveElapsedSeconds = 0.f;
    MoveStarted = false;
}

