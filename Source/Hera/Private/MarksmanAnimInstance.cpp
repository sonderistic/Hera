// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanAnimInstance.h"
#include "MarksmanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMarksmanAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    MarksmanCharacter = Cast<AMarksmanCharacter>(TryGetPawnOwner());
}

void UMarksmanAnimInstance::NativeUpdateAnimation(float DeltaTime) 
{
    Super::NativeUpdateAnimation(DeltaTime);
    
    if (!MarksmanCharacter)
    {
        MarksmanCharacter = Cast<AMarksmanCharacter>(TryGetPawnOwner());
    }

    if (!MarksmanCharacter)
    {
        return;
    }

    FVector Velocity = MarksmanCharacter->GetVelocity();
    Velocity.Z = 0.f;

    if (Velocity.Size() > 3)
    {
        Speed = Velocity.Size();
    }
    else
    {
        Speed = 0;
    }
    /*
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            2,
            FColor::Yellow,
            FString::Printf(TEXT("Speed %f"), Speed)
        );
    }
*/
    bIsInAir = MarksmanCharacter->GetCharacterMovement()->IsFalling();
    
    bIsAccelerating = MarksmanCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 3 ? true : false; 
}